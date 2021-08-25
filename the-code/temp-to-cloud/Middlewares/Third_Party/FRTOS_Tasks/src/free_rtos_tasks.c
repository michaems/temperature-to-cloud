#include "free_rtos_tasks.h"


SemaphoreHandle_t  handleMutexForFlashMemoryReadWrite;

char data_A[100] = "*****SAVE_TEMPR_TO_FLASH: MUTEX AQCUIRED****\r\n\0";
char data_B[100] = "*****READ_DATA_FROM_FLASH: MUTEX AQCUIRED****\r\n\0";

extern UART_HandleTypeDef huart3;

void FreeRTOS_InitTasks()
{

	handleMutexForFlashMemoryReadWrite = xSemaphoreCreateMutex();

	if (handleMutexForFlashMemoryReadWrite == NULL)
	{
		while(1);
	}

	//Task for geting current time from time server*/
	/*if (xTaskCreate(vProvideTimeFromTimeServer, "PROVIDE_DATETIME",
					128, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
	{

		while(1);
	}*/

	/*Task for geting current time from time server*/
	if (xTaskCreate(vMonitorAndSaveTemperatureSensorReadings, "SAVE_TEMPR_TO_FLASH",
					128, NULL, tskIDLE_PRIORITY + 4, NULL) != pdPASS)
	{

		while(1);
	}

	if (xTaskCreate(vSendTemperatureDataToExternalNetowrk, "READ_DATA_FROM_FLASH",
						128, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
	{
		while(1);
	}

}

void vMonitorNetworkConnectionStatus(void *pvParameters)
{
	for(;;)
	{

	}
}

void vMonitorAndSaveTemperatureSensorReadings(void *pvParameters)
{
	char tempr[2] = {0};

	static uint32_t encoded_data[2];
	struct tempr_sensor_data tempr_data;

	for(;;)
	{
		DS1631_ReadTemperature(tempr, &tempr_data.tempre_fixed, &tempr_data.tempre_flpart);

		tempr_data.sequence_number = GetAvalableSequenceNumber();

		tempr_data.timestmp = Rtc_Get_Timestamp();

		/*Encoding data for saving*/
		memset((void *)encoded_data, 0, 2*sizeof(uint32_t));
		EncodeDataToByteArray(&tempr_data, encoded_data);

		if (xSemaphoreTake(handleMutexForFlashMemoryReadWrite, 200/portTICK_PERIOD_MS) == pdPASS)
		{
			HAL_UART_Transmit(&huart3, (uint8_t *)data_A, strlen(data_A), 1000);
			SaveDataToFlash(encoded_data, ENCODED_DATA_SIZE_BYTES / sizeof(uint32_t));


			HAL_UART_Transmit(&huart3, (uint8_t *)tempr, strlen(tempr), 1000);
			HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\0", strlen("\r\n\0"), 1000);
			HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\0", strlen("\r\n\0"), 1000);

			xSemaphoreGive(handleMutexForFlashMemoryReadWrite);
		}

		CraateAndDispatchWriteIndicatorMsg();

		CreateAndDispatchLcdDisplayMessage();

		CreateAndDispatchTerminalMessage();

		vTaskDelay(100);

	}
}

void vReceiveAndPrintMessages(void *pvParmeters)
{
	for(;;)
	{

	}
}
void vSendTemperatureDataToExternalNetowrk(void *pvParmerters)
{
	struct tempr_sensor_data tempr_data;

	uint32_t read_status = HAL_ERROR;
	uint32_t encoded_data[2] = {0};

	char data_to_user[100] = {0};
	char date_and_time[20] = {0};

	for(;;)
	{
		memset((void *)encoded_data, 0, 2 * sizeof(uint32_t));

		if (xSemaphoreTake(handleMutexForFlashMemoryReadWrite, 200/portTICK_PERIOD_MS) == pdPASS)
		{
			//HAL_UART_Transmit(&huart3, (uint8_t *)data_B, strlen(data_B), 1000); //todo: remove this
			read_status = ReadDataFromFlash(encoded_data);


			if (read_status == HAL_OK)
			{
				DecodeByteArrayToData(encoded_data, &tempr_data);

				(void)ConvertTimestampToString(tempr_data.timestmp, date_and_time, 20);

				sprintf(data_to_user, " Sequence number: %lu Temperature: %d.%d ",
						tempr_data.sequence_number, tempr_data.tempre_fixed, tempr_data.tempre_flpart);

				HAL_UART_Transmit(&huart3, (uint8_t *)date_and_time, strlen(date_and_time), 10000);
				HAL_UART_Transmit(&huart3, (uint8_t *)data_to_user, 100, 10000);

			    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\0", strlen("\r\n\0"), 1000);
			    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n\0", strlen("\r\n\0"), 1000);
			}

			xSemaphoreGive(handleMutexForFlashMemoryReadWrite);

		}

		vTaskDelay(100);
	}
}


void vProvideTimeFromTimeServer(void *pvParameters)
{
	UdpClientConnect();

	for(;;)
	{
		while (Get_Ntp_Time_Status() == 0)
		{
			UdpClientSend();

			vTaskDelay(1000);
		}

		HAL_UART_Transmit(&huart3, (uint8_t *)data_A, strlen(data_A), 1000);
		vTaskDelay(600);
		//vTaskDelete(NULL);
	}
}


