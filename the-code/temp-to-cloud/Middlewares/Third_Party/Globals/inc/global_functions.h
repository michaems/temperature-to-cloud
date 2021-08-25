#ifndef THIRD_PARTY_GLOBALS_INC_GLOBAL_FUNCTIONS_H_
#define THIRD_PARTY_GLOBALS_INC_GLOBAL_FUNCTIONS_H_

#include "string.h"
#include "time.h"
#include "stdlib.h"

/* 16 bytes or 128 bits*/
static const uint8_t ENCODED_DATA_SIZE_BYTES = 8;

struct tempr_sensor_data
{
	uint32_t sequence_number;
	time_t   timestmp;
	int tempre_fixed;
	int tempre_flpart;
};

void Set_Ntp_Time_Acquired_From_Time_Server(void);
void Reset_Ntp_Time_Acquired_From_Time_Server(void);

uint8_t Get_Ntp_Time_Status(void);

uint32_t GetAvalableSequenceNumber(void);
void SetRunningSequenceNumber(uint32_t seq_number);

void CraateAndDispatchWriteIndicatorMsg(void);

void CreateAndDispatchLcdDisplayMessage(void);

void CreateAndDispatchTerminalMessage(void);

void EncodeDataToByteArray(struct tempr_sensor_data *tmpr_data, uint32_t *encoded_data);
void DecodeByteArrayToData(uint32_t *encoded_data, struct tempr_sensor_data *tempr_data);

uint8_t ConvertTimestampToString(const time_t timestmp, char *strTime, uint8_t str_len);

#endif /* THIRD_PARTY_GLOBALS_INC_GLOBAL_FUNCTIONS_H_ */
