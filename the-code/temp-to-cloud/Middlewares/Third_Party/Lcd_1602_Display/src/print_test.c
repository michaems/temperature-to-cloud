#include "print_test.h"

void print_test(void)
{
    char * line_1 = "Good Morning ...";
	char * line_2 = "Hi There!";
	char * line_3 = "Good Afternoon.";
	char * line_4 = "How Are You?";
	char * line_5 = "Can you see me?";


	lcd_1602_clear();
	lcd_1602_send_string("READY!\0");
	lcd_1602_delay_ms(1000);

	lcd_1602_clear();
	lcd_1602_set_cursor(0, 0);
	lcd_1602_send_string(line_1);
	lcd_1602_set_cursor(1, 0);
	lcd_1602_send_string(line_2);

	lcd_1602_delay_ms(1000);

	lcd_1602_clear();

	lcd_1602_set_cursor(0, 0);
	lcd_1602_send_string(line_3);

	lcd_1602_set_cursor(1, 0);
	lcd_1602_send_string(line_4);

	lcd_1602_delay_ms(1000);

	lcd_1602_clear();

	lcd_1602_set_cursor(0,0);
	lcd_1602_send_string(line_5);

	lcd_1602_delay_ms(1000);

	lcd_1602_clear();
	lcd_1602_set_cursor(0, 0);
	lcd_1602_send_string("****Good Bye****");
	lcd_1602_set_cursor(1, 5);
	lcd_1602_send_string("THE END");
}

