//
// Created by gabuniku on 2023/04/09.
//

#ifndef ACM0802C_H
#define ACM0802C_H

#include "main.h"


#define ROWS 2
#define COLS 8

#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define ENTRY_MODE 0x04
#define DISPLAY_ON_OFF 0x08
#define CURSOR_OR_DISPLAY_SHIFT 0x10
#define FUNCTION_SET 0x20
#define SET_CGRAM_ADDR 0x40
#define SET_DDRAM_ADDR 0x80

#define MAX_TEXT_LENGTH = ROWS * COLS;

typedef enum {
    BLINK_OFF = 0x00,
    BLINK_ON = 0x01,
    CURSOR_OFF = 0x00,
    CURSOR_ON = 0x02,
    DISPLAY_OFF = 0x00,
    DISPLAY_ON = 0x04,
} DISPLAY_CONF;

typedef enum {
    SHIFT_DISPLAY = 0x08,
    SHIFT_CURSOR = 0x00,
    SHIFT_RIGHT = 0x04,
    SHIFT_LEFT = 0x00,
} SHIFT_CONF;

typedef struct {
    uint8_t row;
    uint8_t col;
} cursor_t;


typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
    cursor_t cursor;
    uint8_t display_conf;
    uint8_t shift_conf;
} acm0802c_t;

void acm0802c_Begin(acm0802c_t *lcd);

void acm0802c_Render(acm0802c_t *lcd, const char *text);

void acm0802c_Home(acm0802c_t *lcd);

void acm0802c_Clear(acm0802c_t *lcd);

void acm0802c_SetCursor(acm0802c_t *lcd, uint8_t col, uint8_t row);

void acm0802c_CursorOn(acm0802c_t *lcd);

void acm0802c_CursorOff(acm0802c_t *lcd);

void acm0802c_BlinkOn(acm0802c_t *lcd);

void acm0802c_BlinkOff(acm0802c_t *lcd);

#endif //ACM0802C_H
