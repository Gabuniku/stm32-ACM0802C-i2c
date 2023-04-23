//
// Created by gabuniku on 2023/04/18.
//

#include "acm0802c.h"

static void sendCommand(acm0802c_t *lcd, uint8_t cmd) {
    // RS = 0
    HAL_I2C_Mem_Write(lcd->hi2c, lcd->address, 0b00000000, 1, &cmd, 1, 10);
}

static void sendData(acm0802c_t *lcd, uint8_t data) {
    // RS = 1
    HAL_I2C_Mem_Write(lcd->hi2c, lcd->address, 0b01000000, 1, &data, 1, 10);
}

void acm0802c_Begin(acm0802c_t *lcd) {
    lcd->cursor = (cursor_t) {0, 0};
    lcd->display_conf = DISPLAY_ON;
    lcd->shift_conf = SHIFT_CURSOR | SHIFT_RIGHT;
    sendCommand(lcd, 0b00111100);
    HAL_Delay(1);
    sendCommand(lcd, DISPLAY_ON_OFF | lcd->display_conf);
    sendCommand(lcd, CURSOR_OR_DISPLAY_SHIFT | lcd->shift_conf);
    sendCommand(lcd, ENTRY_MODE | 0x02);
    sendCommand(lcd, CLEAR_DISPLAY);
    HAL_Delay(1);
}

void acm0802c_Render(acm0802c_t *lcd, const char *text) {
    int i = 0;
    while (text[i] != '\0') {
        if (lcd->cursor.col < COLS) {
            sendData(lcd, text[i]);
            lcd->cursor.col++;
        } else {
            if (lcd->cursor.row == ROWS - 1) {
                acm0802c_SetCursor(lcd, 0, 0);
            } else {
                acm0802c_SetCursor(lcd, 0, ROWS - 1);
            }
        }
        i++;
    }
}

void acm0802c_Home(acm0802c_t *lcd) {
    sendCommand(lcd, RETURN_HOME);
    HAL_Delay(2);
    lcd->cursor.col = 0;
    lcd->cursor.row = 0;
}

void acm0802c_Clear(acm0802c_t *lcd) {
    sendCommand(lcd, CLEAR_DISPLAY);
    HAL_Delay(1);
}

void acm0802c_SetCursor(acm0802c_t *lcd, uint8_t col, uint8_t row) {
    lcd->cursor.col = col < COLS ? col : COLS - 1;
    lcd->cursor.row = row < ROWS ? row : ROWS - 1;
    uint8_t row_offset = lcd->cursor.row == ROWS - 1 ? 0x40 : 0x00;
    sendCommand(lcd, SET_DDRAM_ADDR | (row_offset + lcd->cursor.col));
}


void acm0802c_CursorOn(acm0802c_t *lcd) {
    lcd->display_conf |= CURSOR_ON;
    sendCommand(lcd, DISPLAY_ON_OFF | lcd->display_conf);
    HAL_Delay(1);
}

void acm0802c_CursorOff(acm0802c_t *lcd) {
    lcd->display_conf &= ~CURSOR_ON;
    sendCommand(lcd, DISPLAY_ON_OFF | lcd->display_conf);
    HAL_Delay(1);
}


void acm0802c_BlinkOn(acm0802c_t *lcd) {
    lcd->display_conf |= BLINK_ON;
    sendCommand(lcd, DISPLAY_ON_OFF | lcd->display_conf);
    HAL_Delay(1);
}

void acm0802c_BlinkOff(acm0802c_t *lcd) {
    lcd->display_conf &= ~BLINK_OFF;
    sendCommand(lcd, DISPLAY_ON_OFF | lcd->display_conf);
    HAL_Delay(1);
}
