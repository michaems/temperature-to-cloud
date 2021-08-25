#ifndef THIRD_PARTY_SAVE_TO_FLASH_INC_SAVE_TO_FLASH_H_
#define THIRD_PARTY_SAVE_TO_FLASH_INC_SAVE_TO_FLASH_H_

#include "main.h"
#include "string.h"

void InitFlashSector(void);

uint32_t SaveDataToFlash(uint32_t *data, uint8_t number_of_words);
uint32_t ReadDataFromFlash(uint32_t *encoded_data);

uint32_t CheckMemLimits(uint32_t current_memory_address);

uint32_t EraseFlashSector(void);

#endif /* THIRD_PARTY_SAVE_TO_FLASH_INC_SAVE_TO_FLASH_H_ */
