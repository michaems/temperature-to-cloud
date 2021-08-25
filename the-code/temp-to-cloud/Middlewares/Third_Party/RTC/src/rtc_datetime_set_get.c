#include "rtc_datetime_set_get.h"
#include "stdio.h"

void Rtc_Set_DateTime(void) {

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	/**
	 * Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 14;
	sTime.Minutes = 10;
	sTime.Seconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
	sDate.Month = RTC_MONTH_AUGUST;
	sDate.Date = 19;
	sDate.Year = 21;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);

}

void Rtc_Set_DateTime_With_TimeStamp(struct tm timestamp)
{

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	/**
	 * Initialize RTC and set the Time and Date
	 */
	sTime.Hours = timestamp.tm_hour;
	sTime.Minutes = timestamp.tm_min;
	sTime.Seconds = timestamp.tm_sec;

	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	sDate.WeekDay = timestamp.tm_wday;
	sDate.Month   = timestamp.tm_mon;
	sDate.Date    = timestamp.tm_mday;
	sDate.Year    = timestamp.tm_year;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
}

void Rtc_Get_DateTime(char *time, char *date) {
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	sprintf(time, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	sprintf(date, "%02d-%02d-%02d", sDate.Date, sDate.Month, sDate.Year + 2000);
}

time_t Rtc_Get_Timestamp(void)
{
	time_t timestmp;
	struct tm current_time;

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	current_time.tm_year = sDate.Year + 100;
	current_time.tm_mday = sDate.Date;
	current_time.tm_mon  = sDate.Month - 1;

	current_time.tm_hour = sTime.Hours;
	current_time.tm_min  = sTime.Minutes;
	current_time.tm_sec  = sTime.Seconds;

	timestmp = mktime(&current_time);

	return timestmp;
}
