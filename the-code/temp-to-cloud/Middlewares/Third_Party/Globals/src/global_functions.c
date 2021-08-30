#include "stdio.h"
#include "global_functions.h"

uint8_t ntp_time_acquired = 0;

uint32_t sequence_number_for_saving_ops = 0;

extern UART_HandleTypeDef huart3;

void Set_Ntp_Time_Acquired_From_Time_Server(void)
{
	ntp_time_acquired = 1;
}
void Reset_Ntp_Time_Acquired_From_Time_Server(void)
{
	ntp_time_acquired = 0;
}

uint8_t Get_Ntp_Time_Status(void)
{
	return ntp_time_acquired;
}

uint32_t GetAvalableSequenceNumber(void)
{
	return ++sequence_number_for_saving_ops;
}

void SetRunningSequenceNumber(uint32_t seq_number)
{
	sequence_number_for_saving_ops = seq_number;
}

void EncodeDataToByteArray(struct tempr_sensor_data *tmpr_data, uint32_t *encoded_data)
{
	int8_t   tempre_fixed = tmpr_data->tempre_fixed;
	uint8_t  tempre_flpart = tmpr_data->tempre_flpart;
	uint32_t seq_number = tmpr_data->sequence_number;
	time_t   tmstmp = tmpr_data->timestmp;

	encoded_data[0] |= 0;
	encoded_data[0] |= (seq_number << 16);
	encoded_data[0] |= (tempre_fixed << 8);
	encoded_data[0] |= (tempre_flpart << 0);

	encoded_data[1] = tmstmp;
}

void DecodeByteArrayToData(uint32_t *encoded_data, struct tempr_sensor_data *tempr_data)
{
	tempr_data->sequence_number = encoded_data[0] >> 16;
	tempr_data->tempre_fixed    = ((encoded_data[0] & (0xFF << 8)) >> 8);
	tempr_data->tempre_flpart   = (encoded_data[0] & 0xFF);

	tempr_data->timestmp = encoded_data[1];
}

uint8_t ConvertTimestampToString(const time_t timestmp, char *strTime, uint8_t str_len)
{
	static struct tm *tme;

	tme = localtime(&timestmp);

	/* This function causes hardware fault - why? */
	//strftime(strTime, str_len, "%d-%m-%y %X", (const struct tm*)tme);

	sprintf(strTime,
			"%02d-%02d-%d %02d:%02d:%02d",
			tme->tm_mday, tme->tm_mon + 1,
			tme->tm_year - 100 + 2000,
			tme->tm_hour, tme->tm_min,
			tme->tm_sec);

	free((void *)tme);
	tme = NULL;

	return strlen(strTime);
}

uint8_t CreateTerminalMessage(const char * title, const struct tempr_sensor_data * sensor_data, struct user_message * msg)
{
	static char data_to_user[100] = {0};
	static char date_and_time[20] = {0};

	(void)ConvertTimestampToString(sensor_data->timestmp, date_and_time, sizeof(date_and_time));

	sprintf(data_to_user, "%s | Record time %s Sequence number: %lu Temperature: %d.%d", title, date_and_time,
			sensor_data->sequence_number, sensor_data->tempre_fixed, sensor_data->tempre_flpart);

	msg->msg_type = MSG_TYPE_TERMINAL_MESSAGE;
	memset((void *)msg->msg_txt, 0, sizeof(msg->msg_txt));
	memcpy((void *)msg->msg_txt, (const void *)data_to_user, strlen(data_to_user));

	return 0;
}

uint8_t CreateLcdMessage(const struct tempr_sensor_data * sensor_data, struct user_message * msg)
{
	static char data_to_user[100] = {0};
	static char date_and_time[20] = {0};

	(void)ConvertTimestampToString(sensor_data->timestmp, date_and_time, sizeof(date_and_time));

	memmove(&date_and_time[6], &date_and_time[7], strlen(date_and_time) - 6);
	memmove(&date_and_time[6], &date_and_time[7], strlen(date_and_time) - 6);
	date_and_time[14] = '\0';
	sprintf(data_to_user, "%s#N:%lu C: %d.%d", date_and_time,
			sensor_data->sequence_number, sensor_data->tempre_fixed, sensor_data->tempre_flpart);

	msg->msg_type = MSG_TYPE_LCD_MESSAGE;

	memset((void *)msg->msg_txt, 0, sizeof(msg->msg_txt));
	memcpy((void *)msg->msg_txt, (const void *)data_to_user, strlen(data_to_user));

	return 0;
}

uint8_t CreateLedMessage(uint8_t msg_type, struct user_message * msg)
{
	uint8_t status = HAL_OK;

	if (msg_type == MSG_TYPE_WRITE_FLASH_BLUE_LED)
	{
		msg->msg_type = MSG_TYPE_WRITE_FLASH_BLUE_LED;
		msg->msg_on_duration_ms = 25;
		msg->msg_off_duration_ms = 25;
		msg->msg_repetition = 2;
	}
	else if (msg_type == MSG_TYPE_READ_FLASH_GREEN_LED)
	{
		msg->msg_type = MSG_TYPE_READ_FLASH_GREEN_LED;
		msg->msg_on_duration_ms = 25;
		msg->msg_off_duration_ms = 25;
		msg->msg_repetition = 2;
	}
	else
	{
		status = HAL_ERROR;
	}

	return status;
}

void PrintMsgToLcd(const struct user_message * msg)
{
	static char lcd_line_1[16] = {0};
	static char lcd_line_2[16] = {0};

	int index_of_hash = (int) (strchr(msg->msg_txt, '#') - msg->msg_txt);
	strncpy(lcd_line_1, msg->msg_txt, index_of_hash);

	strncpy(lcd_line_2, msg->msg_txt + index_of_hash+1, strlen(msg->msg_txt) - index_of_hash);

	lcd_1602_set_cursor(0, 0);
	lcd_1602_send_string(lcd_line_1);
	lcd_1602_set_cursor(1, 0);
	lcd_1602_send_string(lcd_line_2);}

void PrintMsgToTerminal(const struct user_message * received_msg)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)received_msg->msg_txt, strlen(received_msg->msg_txt), 10000);
	HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\0", strlen("\r\n\0"), 1000);
	HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\0", strlen("\r\n\0"), 1000);
}

void BlinkBlueLed(const struct user_message * msg)
{
	for (int i = 0; i < msg->msg_repetition; i++)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_Delay(msg->msg_on_duration_ms);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_Delay(msg->msg_off_duration_ms);
	}
}

void BlinkGreenLed(const struct user_message * msg)
{
	for (int i = 0; i < msg->msg_repetition; i++)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_Delay(msg->msg_on_duration_ms);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_Delay(msg->msg_off_duration_ms);
	}
}
