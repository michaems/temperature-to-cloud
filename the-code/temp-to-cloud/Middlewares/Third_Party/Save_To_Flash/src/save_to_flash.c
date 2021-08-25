#include "save_to_flash.h"

/* Sector 11 256 KB*/
static const uint32_t flash_sector_start_address = 0x081C0000;
static const uint32_t flash_sector_end_address   = 0x081FFFFF;

const uint32_t number_of_words = 2;

static uint32_t flash_sector_currently_available_address = flash_sector_start_address;
static volatile uint32_t flash_sector_read_address = flash_sector_start_address;

static int8_t sector_inited = 0;
static int8_t sector_is_full = 0;

uint32_t ReadDataFromFlash(uint32_t *encoded_data)
{
	uint32_t status = HAL_ERROR;

	uint32_t requested_data[number_of_words];

	if (CheckMemLimits(flash_sector_read_address) == HAL_ERROR)
	{
		flash_sector_read_address = flash_sector_start_address;
	}

	memset((void*)requested_data, 0, number_of_words * sizeof(uint32_t));
	memcpy((void*) requested_data, (const void*)flash_sector_read_address, number_of_words * sizeof(uint32_t));

	if (requested_data[0] != 0xFFFFFFFF)
	{
		flash_sector_read_address += (number_of_words * sizeof(uint32_t));
		memcpy((void*)encoded_data, (const void*)requested_data, number_of_words * sizeof(uint32_t));
		status = HAL_OK;
	}
	else
	{
		//flash_sector_read_address = flash_sector_start_address;
	}

	return status;
}

uint32_t SaveDataToFlash(uint32_t *data, uint8_t number_of_words)
{

	if (!sector_inited)
	{
		InitFlashSector();
	}

	if (sector_is_full)
	{
		uint32_t eraseResult = EraseFlashSector();

		if (eraseResult != HAL_OK)
		{
			return eraseResult;
		}
	}

	uint32_t data_to_be_saved[2];

	memcpy((void *)data_to_be_saved, (const void *)data, 2*sizeof(uint32_t));

	//Save given data to currently avalable address
	HAL_FLASH_Unlock();

	for (int i = 0; i < number_of_words; i++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_sector_currently_available_address, data_to_be_saved[i] ) == HAL_OK)
		{
			flash_sector_currently_available_address += sizeof(uint32_t);

			if (CheckMemLimits(flash_sector_currently_available_address) == HAL_ERROR)
			{
				break;
			}
		}
		else
		{
			return HAL_FLASH_GetError();
		}
	}

	HAL_FLASH_Lock();

	return HAL_OK;
}

void InitFlashSector(void)
{

	uint32_t data[number_of_words];

	do
	{
		memcpy((void *)data,(const void *)(flash_sector_currently_available_address), number_of_words * sizeof(uint32_t));

		if (CheckMemLimits(flash_sector_currently_available_address) != HAL_OK)
		{
			break;
		}

		if (data[0] != 0xFFFFFFFF)
		{
			flash_sector_currently_available_address += (number_of_words * sizeof(uint32_t));
		}


	}while (data[0] != 0xFFFFFFFF);

	sector_inited = 1;

}

uint32_t EraseFlashSector(void)
{
	uint32_t eraseResult = HAL_OK;

	static FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t sectorError;

	eraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
	eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	eraseInitStruct.Sector       = FLASH_SECTOR_11;
	eraseInitStruct.NbSectors    = 1;

	HAL_FLASH_Unlock();

	if (HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError) != HAL_OK)
	{
		eraseResult = HAL_FLASH_GetError();
	}
	else
	{
		sector_is_full = 0;
		flash_sector_currently_available_address = flash_sector_start_address;
	}

	HAL_FLASH_Lock();

	return eraseResult;
}

uint32_t CheckMemLimits(uint32_t current_memory_address)
{
	uint32_t result = HAL_OK;

	if (flash_sector_currently_available_address >= (flash_sector_end_address - (4 * sizeof(uint32_t))))
	{
		flash_sector_currently_available_address = flash_sector_start_address;
		sector_is_full = 1;
		result = HAL_ERROR;

	}

	return result;
}
