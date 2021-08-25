#include "stdio.h"
#include "global_functions.h"

uint8_t ntp_time_acquired = 0;

uint32_t sequence_number_for_saving_ops = 0;

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
	struct tm *tme;

	tme = localtime(&timestmp);
	memset((void*)strTime, 0, str_len);
	//strftime(strTime, str_len, "%d-%m-%y %X", (const struct tm*)tme);
	sprintf(strTime,
			"%02d-%02d-%d %02d:%02d:%02d",
			tme->tm_mday, tme->tm_mon + 1,
			tme->tm_year - 100 + 2000,
			tme->tm_hour, tme->tm_min,
			tme->tm_sec),

	free((void *)tme);

	return strlen(strTime);
}

void CraateAndDispatchWriteIndicatorMsg(void)
{

}

void CreateAndDispatchLcdDisplayMessage(void)
{

}

void CreateAndDispatchTerminalMessage(void)
{

}


