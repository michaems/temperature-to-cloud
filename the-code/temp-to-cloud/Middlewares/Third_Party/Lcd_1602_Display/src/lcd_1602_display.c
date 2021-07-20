#include <lcd_1602_display.h>
#include "stm32F7xx_hal.h"
#include "lcd_1602_stm32f7xx_config.h"

void lcd_1602_clear(void)
{
	char clear_cmd_1 = 0x00;
	char clear_cmd_2 = 0x01;

	lcd_1602_send_cmd(clear_cmd_1);
	lcd_1602_send_cmd(clear_cmd_2);

	//do not blink the cursor (0x0F if blink is needed)
	lcd_1602_send_cmd(0x0C);
}

void lcd_1602_init(void)
{
	config_stm32f7xx_for_lcd_1602();

	char function_set_00   = 0x20;
	char function_set_01   = 0x20;
	char function_set_02   = 0xC0; //2 lines, font=5x7 dots

	char display_ONOFF_10  = 0x00;
	char display_ONOFF_11  = 0x0F;

	char display_clear_20  = 0x00;
	char display_clear_21  = 0x01;

	char entry_mode_30     = 0x00;
	char entry_mode_31     = 0x60;

	//function set
	lcd_1602_delay_ms(35);
	lcd_1602_send_cmd(function_set_00);
	lcd_1602_send_cmd(function_set_01);
	lcd_1602_send_cmd(function_set_02);

	//display on/off
	lcd_1602_delay_ms(1);
	lcd_1602_send_cmd(display_ONOFF_10);
	lcd_1602_send_cmd(display_ONOFF_11);

	//display clear
	lcd_1602_delay_ms(1);
	lcd_1602_send_cmd(display_clear_20);
	lcd_1602_send_cmd(display_clear_21);

	//entry mode set
	lcd_1602_delay_ms(1);
	lcd_1602_send_cmd(entry_mode_30);
	lcd_1602_send_cmd(entry_mode_31);

	//Those extra commands are needed
	lcd_1602_set_cursor(0, 0);
    lcd_1602_set_cursor(0, 1);
    lcd_1602_set_cursor(0, 2);
    lcd_1602_set_cursor(0, 3);
    lcd_1602_set_cursor(1, 0);
    lcd_1602_set_cursor(1, 1);
    lcd_1602_set_cursor(1, 2);
    lcd_1602_set_cursor(1, 3);
}

void lcd_1602_send_cmd(char cmd)
{
	int rs            = 0;
	char data_to_send = 0;


	/*Sending upper nibble first*/
	data_to_send = ((cmd >> 4) & 0x0F);
	lcd_1602_send_to_dispay(data_to_send, rs);

	/*Sending lower nibble*/
	data_to_send = ((cmd) & 0x0F);
	lcd_1602_send_to_dispay(data_to_send, rs);

	if (cmd < 4)
		lcd_1602_delay_ms(2);
    else
    	lcd_1602_delay_ms(1);
}

void lcd_1602_send_data(char data)
{
	int rs            = 1;
	char data_to_send = 0;

	/*Sending upper nibble first*/
	data_to_send = ((data >> 4) & 0x0F);
	lcd_1602_send_to_dispay(data_to_send, rs);

	/*Sending lower nibble*/
	data_to_send = ((data) & 0x0F);
	lcd_1602_send_to_dispay(data_to_send, rs);
}

void lcd_1602_set_cursor(int row, int col)
{
	switch (row)
	{
		case 0:
			col |= 0x80;
			break;
		case 1:
			col |= 0xC0;
			break;
	}

	lcd_1602_send_cmd(col);
}

void lcd_1602_send_string(char * str)
{
	while (*str)
	{
		lcd_1602_send_data(*str++);
	}
}

void lcd_1602_send_to_dispay(char data, int rs)
{
	HAL_GPIO_WritePin(RS_GPIO_PORT, RS_PIN, rs);  // rs = 1 for data, rs=0 for command

	lcd_1602_delay_ms(1);

	/* write the data to the respective pin */
	HAL_GPIO_WritePin(D7_GPIO_PORT, D7_PIN, ((data>>3)&0x01));
	HAL_GPIO_WritePin(D6_GPIO_PORT, D6_PIN, ((data>>2)&0x01));
	HAL_GPIO_WritePin(D5_GPIO_PORT, D5_PIN, ((data>>1)&0x01));
	HAL_GPIO_WritePin(D4_GPIO_PORT, D4_PIN, ((data>>0)&0x01));

	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_PIN, 1);
	//lcd_1602_delay_ms(1);
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_PIN, 0);
}

void lcd_1602_backlight_onoff(char onoff)
{
	if (onoff >= 1)
	{
		onoff = 1;
	}
	else
	{
		onoff = 0;
	}

	HAL_GPIO_WritePin(BACKLIGHT_PORT, BACKLIGHT_PIN, onoff);
}

void lcd_1602_delay_ms(int n)
{
	HAL_Delay(n);
}
