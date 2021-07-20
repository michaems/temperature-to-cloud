#include "stm32f7xx_hal.h"
#include "lcd_1602_stm32f7xx_config.h"
#include "lcd_1602_pins_on_stm32f7xx.h"

void config_stm32f7xx_for_lcd_1602(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	 /*Configure GPIO pin for RS*/
	GPIO_InitStruct.Pin =  RS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RS_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin for RW*/
	GPIO_InitStruct.Pin =  RW_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RW_GPIO_PORT, &GPIO_InitStruct);

	 /*Configure GPIO pin for EN*/
	GPIO_InitStruct.Pin =  EN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(EN_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin for GND*/
	GPIO_InitStruct.Pin = LCD_GND;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_GND_PORT, &GPIO_InitStruct);

	 /*Configure GPIO pin for VCC*/
	GPIO_InitStruct.Pin =  LCD_VCC;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD_VCC_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(RS_GPIO_PORT, RS_PIN,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RW_GPIO_PORT, RW_PIN,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN_GPIO_PORT, EN_PIN,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GND_PORT, LCD_GND, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_VCC_PORT, LCD_VCC, GPIO_PIN_SET);

	//Configure data pins
	/*Configure GPIO pin for DB4*/
	GPIO_InitStruct.Pin =  D4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(D4_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin for DB5*/
	GPIO_InitStruct.Pin =  D5_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(D5_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin for DB6*/
	GPIO_InitStruct.Pin =  D6_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(D6_GPIO_PORT, &GPIO_InitStruct);

	/*Configure GPIO pin for DB7*/
	GPIO_InitStruct.Pin = D7_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(D7_GPIO_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(D4_GPIO_PORT, D4_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D5_GPIO_PORT, D5_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D6_GPIO_PORT, D6_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D7_GPIO_PORT, D7_PIN, GPIO_PIN_RESET);

	/*Configure GPIO for PG9*/

	GPIO_InitStruct.Pin =  GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin for PB2*/

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9,  GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  GPIO_PIN_SET);

	GPIO_InitStruct.Pin =  GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin =  GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14,  GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,  GPIO_PIN_RESET);

}


