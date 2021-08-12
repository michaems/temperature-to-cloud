#include "rtc_datetime_set_get.h"


void rtc_Set_DateTime(void) {
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
	sDate.Date = 10;
	sDate.Year = 21;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);

}

void rtc_Get_DateTime(char *time, char *date) {
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	sprintf(time, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	sprintf(date, "%02d-%02d-%02d", sDate.Date, sDate.Month, sDate.Year + 2000);
}
