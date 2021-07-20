#ifndef _LCD_1602_PINS_STM32F7XX_
#define _LCD_1602_PINS_STM32F7XX_

/*Defining LCD 1602 pins*/

/*Backlight is not conneced on PMODCLP*/
#define BACKLIGHT_PIN  GPIO_PIN_15
#define BACKLIGHT_PORT GPIOE

#define RS_PIN       GPIO_PIN_10
#define RS_GPIO_PORT GPIOE

/*No need for RW PIN connected to GND*/
#define RW_PIN       GPIO_PIN_12
#define RW_GPIO_PORT GPIOE

/*EN PIN*/
#define EN_PIN       GPIO_PIN_14
#define EN_GPIO_PORT GPIOE

#define LCD_GND      GPIO_PIN_10
#define LCD_GND_PORT GPIOB

#define LCD_VCC      GPIO_PIN_11
#define LCD_VCC_PORT GPIOB

/*Data pins*/
#define D4_PIN       GPIO_PIN_11
#define D4_GPIO_PORT GPIOE

#define D5_PIN       GPIO_PIN_14
#define D5_GPIO_PORT GPIOF

#define D6_PIN       GPIO_PIN_13
#define D6_GPIO_PORT GPIOE

#define D7_PIN       GPIO_PIN_15
#define D7_GPIO_PORT GPIOF


#endif /*_LCD_1602_PINS_STM32F7XX_*/
