/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  IAR Embedded Workbench for ARM
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef _GLOBAL_H
#define _GLOBAL_H
#include <stdio.h>
#include "stm32f1xx_hal.h"
typedef struct _transducer
{
	int frequency;
	int phase;
	uint8_t number;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
} _transducer;

#define NUMBER_OF_TRANSDUCER 4


typedef struct testData{
	int frequency;
	int phase;
	int number;
}testData;

enum packEnum{
	idleState = 0,
	packetReceived,
	sendAd9833,
	notReceived,
	errorPacket,
};


#define AD9833_SPI_CLK_Pin GPIO_PIN_5
#define AD9833_SPI_CLK_GPIO_Port GPIOA
#define AD9833_SPI_SS_Pin GPIO_PIN_4
#define AD9833_SPI_SS_GPIO_Port GPIOA

#define AD9833_SPI_DATA_Pin GPIO_PIN_6
#define AD9833_SPI_DATA_GPIO_Port GPIOA
#define AD9833_SPI_DATA_Pin2 GPIO_PIN_7
#define AD9833_SPI_DATA_GPIO_Port2 GPIOA
#define AD9833_SPI_DATA_Pin3 GPIO_PIN_0
#define AD9833_SPI_DATA_GPIO_Port3 GPIOB
#define AD9833_SPI_DATA_Pin4 GPIO_PIN_1
#define AD9833_SPI_DATA_GPIO_Port4 GPIOB


#define FMCLK 25000000        // Master Clock On AD9833
#define AD9833PORT GPIOA      // PORT OF AD9833
#define AD9833DATA GPIO_PIN_6 // SPI DATA PIN
#define AD9833SCK GPIO_PIN_5  // SPI Clock PIN
#define AD9833SS GPIO_PIN_4   // SPI Chip Select

#define AD9833PORT2 GPIOA      // PORT OF AD9833
#define AD9833DATA2 GPIO_PIN_7 // SPI DATA PIN
#define AD9833PORT3 GPIOB      // PORT OF AD9833
#define AD9833DATA3 GPIO_PIN_0 // SPI DATA PIN
#define AD9833PORT4 GPIOB      // PORT OF AD9833
#define AD9833DATA4 GPIO_PIN_1 // SPI DATA PIN

#endif
