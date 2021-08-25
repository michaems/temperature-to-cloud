#ifndef THIRD_PARTY_FRTOS_TASKS_INC_FREE_RTOS_TASKS_H_
#define THIRD_PARTY_FRTOS_TASKS_INC_FREE_RTOS_TASKS_H_
#include "stdio.h"
#include "global_functions.h"
#include "time_server_udp_client_raw.h"
#include "rtc_datetime_set_get.h"
#include "temp_sensor_ds1631.h"
#include "save_to_flash.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void FreeRTOS_InitTasks();

void vProvideTimeFromTimeServer(void *pvParameters);

void vMonitorNetworkConnectionStatus(void *pvParameters);
void vMonitorAndSaveTemperatureSensorReadings(void *pvParameters);

void vReceiveAndPrintMessages(void *pvParmeters);
void vSendTemperatureDataToExternalNetowrk(void *pvParmerters);


#endif /* THIRD_PARTY_FRTOS_TASKS_INC_FREE_RTOS_TASKS_H_ */
