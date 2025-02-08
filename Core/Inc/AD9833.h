/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>  
 Software:  IAR Embedded Workbench for ARM
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef _AD_9833_H
#define _AD_9833_H
#include <math.h>
#include "stm32f1xx_hal.h"
#include "global.h"
// ------------------------- Defines -------------------------


//#define ASM_NOP() asm("NOP")  // Assembly NOPE (Little Delay)
enum WaveType{SIN, SQR, TRI}; // Wave Selection Enum
// ------------------ Functions  ---------------------
void AD9833_SetWave(uint16_t Wave);                      // Sets Output Wave Type
void AD9833_SetWaveData(float Frequency, float Frequency2, float Phase, float Phase2);    // Sets Wave Frequency & Phase
void AD9833_Init(uint16_t WaveType, float FRQ, float FRQ2, float Phase, float Phase2);   // Initializing AD9833
void AD9833_Init_Test(uint16_t WaveType, _transducer *myTransducer);
void AD9833_SetWaveData_Test(_transducer *myTransducer);
#endif
