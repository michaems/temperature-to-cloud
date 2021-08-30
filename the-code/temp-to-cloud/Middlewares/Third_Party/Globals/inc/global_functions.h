#ifndef THIRD_PARTY_GLOBALS_INC_GLOBAL_FUNCTIONS_H_
#define THIRD_PARTY_GLOBALS_INC_GLOBAL_FUNCTIONS_H_

#include "string.h"
#include "time.h"
#include "stdlib.h"
#include "lcd_1602_display.h"
#include "stm32f7xx_hal.h"

#define MSG_TYPE_TERMINAL_MESSAGE 1
#define MSG_TYPE_LCD_MESSAGE      2
#define MSG_TYPE_LED_MESSAGE      3

#define MSG_TYPE_READ_FLASH_GREEN_LED 31
#define MSG_TYPE_WRITE_FLASH_BLUE_LED 32
#define MSG_TYPE_ERROR_RED_LED        33



/* 16 bytes or 128 bits*/
static const uint8_t ENCODED_DATA_SIZE_BYTES = 8;

struct tempr_sensor_data
{
	uint32_t sequence_number;
	time_t   timestmp;
	int tempre_fixed;
	int tempre_flpart;
};

struct user_message
{
	uint8_t msg_type;
	uint16_t msg_on_duration_ms;
	uint16_t msg_off_duration_ms;
	uint8_t msg_repetition;
	char msg_txt[100];
};

void Set_Ntp_Time_Acquired_From_Time_Server(void);
void Reset_Ntp_Time_Acquired_From_Time_Server(void);

uint8_t Get_Ntp_Time_Status(void);

uint32_t GetAvalableSequenceNumber(void);
void SetRunningSequenceNumber(uint32_t seq_number);

void EncodeDataToByteArray(struct tempr_sensor_data *tmpr_data, uint32_t *encoded_data);
void DecodeByteArrayToData(uint32_t *encoded_data, struct tempr_sensor_data *tempr_data);

uint8_t ConvertTimestampToString(const time_t timestmp, char *strTime, uint8_t str_len);

uint8_t CreateTerminalMessage(const char * title, const struct tempr_sensor_data * sensor_data, struct user_message * msg);
uint8_t CreateLcdMessage(const struct tempr_sensor_data * sensor_data, struct user_message * msg);
uint8_t CreateLedMessage(uint8_t msg_type, struct user_message * msg);

void PrintMsgToLcd(const struct user_message * msg);
void PrintMsgToTerminal(const struct user_message * received_msg);

void BlinkBlueLed(const struct user_message * msg);
void BlinkGreenLed(const struct user_message * msg);

#endif /* THIRD_PARTY_GLOBALS_INC_GLOBAL_FUNCTIONS_H_ */
