#include "free_rtos_tasks.h"

extern UART_HandleTypeDef huart3;

SemaphoreHandle_t  handleMutexForFlashMemoryReadWrite;
SemaphoreHandle_t  handleSemaphoreForMessageQueue;
QueueHandle_t      ptrUserMessagesQueue;

char data_A[100] = "*****SAVE_TEMPR_TO_FLASH: MUTEX AQCUIRED****\r\n\0";
char data_B[100] = "*****READ_DATA_FROM_FLASH: MUTEX AQCUIRED****\r\n\0";



void FreeRTOS_InitTasks()
{

	handleMutexForFlashMemoryReadWrite = xSemaphoreCreateMutex();

	assert_param(handleMutexForFlashMemoryReadWrite);

	handleSemaphoreForMessageQueue = xSemaphoreCreateBinary();

	assert_param(handleSemaphoreForMessageQueue);

	//Creating queue to hold messages for the user
	ptrUserMessagesQueue = xQueueCreate(25, sizeof(struct user_message));

	assert_param(ptrUserMessagesQueue != NULL);

	if (xTaskCreate(vMonitorAndSaveTemperatureSensorReadings, "SAVE_TEMPR_TO_FLASH",
					128, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
	{
		while(1);
	}

	if (xTaskCreate(vSendTemperatureDataToExternalNetowrk, "READ_DATA_FROM_FLASH",
						128, NULL, tskIDLE_PRIORITY + 10, NULL) != pdPASS)
	{
		while(1);
	}

	if (xTaskCreate(vReceiveAndPrintMessages, "PRINT_MESSAGE",
			128, NULL, tskIDLE_PRIORITY + 3, NULL) != pdPASS)
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

	struct user_message terminal_msg;

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
			SaveDataToFlash(encoded_data, ENCODED_DATA_SIZE_BYTES / sizeof(uint32_t));

			CreateTerminalMessage("FLASH WRITE", &tempr_data, &terminal_msg);

			xQueueSend(ptrUserMessagesQueue, &terminal_msg, portMAX_DELAY);

			xSemaphoreGive(handleSemaphoreForMessageQueue);

			xSemaphoreGive(handleMutexForFlashMemoryReadWrite);
		}

		vTaskDelay(100);

	}
}


void vSendTemperatureDataToExternalNetowrk(void *pvParmerters)
{
	static struct tempr_sensor_data tempr_data;
	static struct user_message terminal_msg;
	static struct user_message lcd_msg;
	uint32_t read_status = HAL_ERROR;
	uint32_t encoded_data[2] = {0};

	for(;;)
	{
		memset((void *)encoded_data, 0, 2 * sizeof(uint32_t));

		if (xSemaphoreTake(handleMutexForFlashMemoryReadWrite, 200/portTICK_PERIOD_MS) == pdPASS)
		{
			read_status = ReadDataFromFlash(encoded_data);

			if (read_status == HAL_OK)
			{
				DecodeByteArrayToData(encoded_data, &tempr_data);

				CreateTerminalMessage("FLASH READ", &tempr_data, &terminal_msg);
				CreateLcdMessage(&tempr_data, &lcd_msg);

				xQueueSend(ptrUserMessagesQueue, &terminal_msg, portMAX_DELAY);
				xSemaphoreGive(handleSemaphoreForMessageQueue);

				xQueueSend(ptrUserMessagesQueue, &lcd_msg, portMAX_DELAY);
				xSemaphoreGive(handleSemaphoreForMessageQueue);
			}

			xSemaphoreGive(handleMutexForFlashMemoryReadWrite);
		}

		vTaskDelay(100);
	}
}


void vReceiveAndPrintMessages(void *pvParmeters)
{
	static struct user_message received_msg;

	for(;;)
	{
		if (xQueueSemaphoreTake(handleSemaphoreForMessageQueue, 500/portTICK_PERIOD_MS) == pdTRUE)
		{
			//Get Mesaage from the queue
			if (xQueueReceive(ptrUserMessagesQueue, &received_msg, portMAX_DELAY) == pdTRUE)
			{
				if (received_msg.msg_type == MSG_TYPE_TERMINAL_MESSAGE)
				{
					PrintMsgToTerminal(&received_msg);
				}
				else if (received_msg.msg_type == MSG_TYPE_LCD_MESSAGE)
				{
					PrintMsgToLcd(&received_msg);
				}
				else
				{

				}

			}

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
	}
}

