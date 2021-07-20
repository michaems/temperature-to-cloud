#ifndef _LED_1602_DISPLAY_H_
#define _LED_1602_DISPLAY_H_

#include "lcd_1602_pins_on_stm32f7xx.h"

void lcd_1602_init(void);

void lcd_1602_send_cmd(char cmd);

void lcd_1602_send_data(char data);

/*sets cursor to entered position: (row 0 or 1), (column 0-15)*/
void lcd_1602_set_cursor(int row, int col);

void lcd_1602_clear(void);

void lcd_1602_send_string(char * str);

void lcd_1602_send_to_dispay(char data, int rs);

void lcd_1602_backlight_onoff(char onoff);

void lcd_1602_delay_ms(int n);

#endif /*_LED_1602_DISPLAY_H_*/
