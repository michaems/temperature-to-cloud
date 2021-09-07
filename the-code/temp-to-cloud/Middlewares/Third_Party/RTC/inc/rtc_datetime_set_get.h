#ifndef THIRD_PARTY_RTC_INC_RTC_DATETIME_SET_GET_H_
#define THIRD_PARTY_RTC_INC_RTC_DATETIME_SET_GET_H_

#include "time.h"
#include "rtc.h"


void Rtc_Set_DateTime(void);
void Rtc_Set_DateTime_With_TimeStamp(struct tm timestamp);

void Rtc_Get_DateTime(char *time, char *date);
time_t Rtc_Get_Timestamp(void);

#endif /* THIRD_PARTY_RTC_INC_RTC_DATETIME_SET_GET_H_ */
