#include "temp_sensor_ds1631.h"


static  uint16_t DS1631_ADDRESS = (0x90 << 0);
static  uint8_t  DS1631_COMMAND_START = 0x51;
static  uint8_t DS1631_COMMAND_READ_TEMPERATURE = 0xAA;

uint8_t received_data[2];

void DS1631_ReadTemperature(char *temerature_str, int *tempr_fixed, int *tempr_flpart)
{
	uint16_t pr = 0;

	memset(received_data, 0, 2);

	HAL_I2C_Master_Transmit(&hi2c2, DS1631_ADDRESS, &DS1631_COMMAND_START, 1, 500);
	HAL_Delay(500);
	HAL_I2C_Master_Transmit(&hi2c2, DS1631_ADDRESS, &DS1631_COMMAND_READ_TEMPERATURE, 1, 500);
	HAL_I2C_Master_Receive(&hi2c2,  DS1631_ADDRESS, received_data, 2, 500);

	pr = (received_data[0] << 8);
	pr |= received_data[1];

	float tempr = (float)pr / 256;

	*tempr_fixed  = (int)(tempr);
	*tempr_flpart = (int)((tempr - (int)tempr)*10);

	sprintf(temerature_str, "%d.%d", *tempr_fixed, *tempr_flpart);
}
