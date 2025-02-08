/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  IAR Embedded Workbench for ARM
 Hardware:  Any STM32 device
*************************************************************************************/
#include "AD9833.h"
//#include "main.h"
// ------------------- Variables ----------------
uint16_t FRQLW = 0;				// MSB of Frequency Tuning Word
uint16_t FRQHW = 0;				// LSB of Frequency Tuning Word
uint32_t phaseVal = 0;			// Phase Tuning Value
uint16_t FRQLW2 = 0;			// MSB of Frequency Tuning Word
uint16_t FRQHW2 = 0;			// LSB of Frequency Tuning Word
uint32_t phaseVal2 = 0;			// Phase Tuning Value
uint8_t WKNOWN = 0;				// Flag Variable
uint16_t FRQLW_ARR[4] = {0};	// MSB of Frequency Tuning Word
uint16_t FRQHW_ARR[4] = {0};	// LSB of Frequency Tuning Word
uint32_t phaseVal_Arr[4] = {0}; // Phase Tuning Value

// -------------------------------- Functions --------------------------------

// ------------------------------------------------ Software SPI Function
void writeSPI(uint16_t word, uint16_t word2)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		if (word & 0x8000)
		{
			HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET); // bit=1, Set High
																	 // HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET); // bit=1, Set High
		}
		else
		{
			HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_RESET); // bit=0, Set Low
																	   // HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET); // bit=0, Set Low
		}
		if (word2 & 0x8000)
		{
			// HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET);   // bit=1, Set High
			HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET); // bit=1, Set High
		}
		else
		{
			// HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_RESET);	 // bit=0, Set Low
			HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET); // bit=0, Set Low
		}
		__NOP();
		HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_RESET); // Data is valid on falling edge
		__NOP();
		HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_SET);
		word = word << 1;	// Shift left by 1 bit
		word2 = word2 << 1; // Shift left by 1 bit
	}
	HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_RESET); // Idle low
	HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET);
	__NOP();
}
void writeSPI_test(uint16_t word, uint16_t word2, int16_t word3, uint16_t word4)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		//HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, (word & 0x8000) ? 1:0);
		//HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, (word2 & 0x8000) ? 1:0);
		//HAL_GPIO_WritePin(AD9833PORT3, AD9833DATA3, (word3 & 0x8000) ? 1:0); 
		//HAL_GPIO_WritePin(AD9833PORT4, AD9833DATA4, (word4 & 0x8000) ? 1:0); 
		if (word & 0x8000)
		{
			HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET); // bit=1, Set High
																	 // HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET); // bit=1, Set High
		}
		else
		{
			HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_RESET); // bit=0, Set Low
																	   // HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET); // bit=0, Set Low
		}
		if (word2 & 0x8000)
		{
			// HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET);   // bit=1, Set High
			HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET); // bit=1, Set High
		}
		else
		{
			// HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_RESET);	 // bit=0, Set Low
			HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET); // bit=0, Set Low
		}
		if (word3 & 0x8000)
		{
			HAL_GPIO_WritePin(AD9833PORT3, AD9833DATA3, GPIO_PIN_SET); // bit=1, Set High
																	   // HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET); // bit=1, Set High
		}
		else
		{
			HAL_GPIO_WritePin(AD9833PORT3, AD9833DATA3, GPIO_PIN_RESET); // bit=0, Set Low
																		 // HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET); // bit=0, Set Low
		}
		if (word4 & 0x8000)
		{
			// HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET);   // bit=1, Set High
			HAL_GPIO_WritePin(AD9833PORT4, AD9833DATA4, GPIO_PIN_SET); // bit=1, Set High
		}
		else
		{
			// HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_RESET);	 // bit=0, Set Low
			HAL_GPIO_WritePin(AD9833PORT4, AD9833DATA4, GPIO_PIN_RESET); // bit=0, Set Low
		}
		__NOP();
		HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_RESET); // Data is valid on falling edge
		for (size_t i = 0; i < 18; i++)
		{
			/* code */
			__NOP();
		}
		
		HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_SET);
		word = word << 1;	// Shift left by 1 bit
		word2 = word2 << 1; // Shift left by 1 bit
		word3 = word3 << 1; // Shift left by 1 bit
		word4 = word4 << 1; // Shift left by 1 bit
	}
	HAL_GPIO_WritePin(AD9833PORT, AD9833DATA | AD9833DATA2, GPIO_PIN_RESET); // Idle low
	// HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AD9833PORT3, AD9833DATA3 | AD9833DATA4, GPIO_PIN_RESET); // Idle low
	// HAL_GPIO_WritePin(AD9833PORT4, AD9833DATA4, GPIO_PIN_RESET);
	__NOP();
}

// ------------------------------------------------ Sets Output Wave Type
void AD9833_SetWave(uint16_t Wave)
{
	switch (Wave)
	{
	case 0:
		HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_RESET);
		writeSPI_test(0x2000, 0x2000, 0x2000, 0x2000); // Value for Sinusoidal Wave
		HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);
		WKNOWN = 0;
		break;
	case 1:
		HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_RESET);
		writeSPI_test(0x2028, 0x2028, 0x2028, 0x2028); // Value for Square Wave
		HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);
		WKNOWN = 1;
		break;
	case 2:
		HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_RESET);
		writeSPI_test(0x2002, 0x2002, 0x2002, 0x2002); // Value for Triangle Wave
		HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);
		WKNOWN = 2;
		break;
	default:
		break;
	}
}

// ------------------------------------------------ Sets Wave Frequency & Phase (In Degree) In PHASE0 & FREQ0 Registers
void AD9833_SetWaveData(float Frequency, float Frequency2, float Phase, float Phase2)
{
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	// ---------- Tuning Word for Phase ( 0 - 360 Degree )
	if (Phase < 0)
	{
		Phase = 0; // Changing Phase Value to Positive
	}

	if (Phase > 360)
	{
		Phase = 360;
	}
	if (Phase2 < 0)
	{
		Phase2 = 0; // Changing Phase Value to Positive
	}

	if (Phase2 > 360)
	{
		Phase2 = 360;
	}
	// Maximum value For Phase (In Degree)
	phaseVal = ((int)(Phase * (4096 / 360))) | 0XC000;	 // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address
	phaseVal2 = ((int)(Phase2 * (4096 / 360))) | 0XC000; // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address

	// ---------- Tuning word for Frequency
	long freq = 0;
	freq = (int)(((Frequency * pow(2, 28)) / FMCLK) + 1); // Tuning Word
	FRQHW = (int)((freq & 0xFFFC000) >> 14);			  // FREQ MSB
	FRQLW = (int)(freq & 0x3FFF);						  // FREQ LSB
	FRQLW |= 0x4000;
	FRQHW |= 0x4000;

	// TransDucer 2:
	//  ---------- Tuning word for Frequency
	long freq2 = 0;
	freq2 = (int)(((Frequency2 * pow(2, 28)) / FMCLK) + 1); // Tuning Word
	FRQHW2 = (int)((freq2 & 0xFFFC000) >> 14);				// FREQ MSB
	FRQLW2 = (int)(freq2 & 0x3FFF);							// FREQ LSB
	FRQLW2 |= 0x4000;
	FRQHW2 |= 0x4000;
	// ------------------------------------------------ Writing DATA
	HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_RESET); // low = selected
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	writeSPI(0x2100, 0x2100); // enable 16bit words and set reset bit
	writeSPI(FRQLW, FRQLW2);
	writeSPI(FRQHW, FRQHW2);
	writeSPI(phaseVal, phaseVal2);
	writeSPI(0x2000, 0x2000); // clear reset bit
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET); // high = deselected
	AD9833_SetWave(WKNOWN);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	return;
}

void AD9833_SetWaveData_Test(_transducer *myTransducer)
{
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	// ---------- Tuning Word for Phase ( 0 - 360 Degree )
	for (int i = 0; i < 4; i++)
	{
		if (myTransducer[i].phase < 0)
		{
			myTransducer[i].phase = 0; // Changing Phase Value to Positive
		}

		if (myTransducer[i].phase > 360)
		{
			myTransducer[i].phase = 360;
		}
		// Maximum value For Phase (In Degree)
		phaseVal_Arr[i] = ((int)(myTransducer[i].phase * (4096 / 360))) | 0XC000; // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address
																				  // ---------- Tuning word for Frequency
		long freq = 0;
		freq = (int)(((myTransducer[i].frequency * pow(2, 28)) / FMCLK) + 1); // Tuning Word
		FRQHW_ARR[i] = (int)((freq & 0xFFFC000) >> 14);						  // FREQ MSB
		FRQLW_ARR[i] = (int)(freq & 0x3FFF);								  // FREQ LSB
		FRQLW_ARR[i] |= 0x4000;
		FRQHW_ARR[i] |= 0x4000;
		HAL_GPIO_WritePin(myTransducer[i].GPIOx, myTransducer[i].GPIO_Pin, GPIO_PIN_SET);
	}

	// ------------------------------------------------ Writing DATA

	HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_RESET); // low = selected
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();

	writeSPI_test(0x2100, 0x2100, 0x2100, 0x2100); // enable 16bit words and set reset bit
	writeSPI_test(FRQLW_ARR[0], FRQLW_ARR[1], FRQLW_ARR[2], FRQLW_ARR[3]);
	writeSPI_test(FRQHW_ARR[0], FRQHW_ARR[1], FRQHW_ARR[2], FRQHW_ARR[3]);
	writeSPI_test(phaseVal_Arr[0], phaseVal_Arr[1], phaseVal_Arr[2], phaseVal_Arr[3]);
	writeSPI_test(0x2000, 0x2000, 0x2000, 0x2000); // clear reset bit
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET); // high = deselected
	AD9833_SetWave(WKNOWN);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	return;
}
// ------------------------------------------------ Initializing AD9833
void AD9833_Init(uint16_t WaveType, float FRQ, float FRQ2, float Phase, float Phase2)
{
	HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET); // Set All SPI pings to High
	HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_SET); // Set All SPI pings to High
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);	// Set All SPI pings to High
	AD9833_SetWave(WaveType);								// Type Of Wave
	AD9833_SetWaveData(FRQ, FRQ2, Phase, Phase2);			// Frequency & Phase Set
	return;
}
void AD9833_Init_Test(uint16_t WaveType, _transducer *myTransducer)
{
	HAL_GPIO_WritePin(AD9833PORT, AD9833DATA, GPIO_PIN_SET); // Set All SPI pings to High
	HAL_GPIO_WritePin(AD9833PORT2, AD9833DATA2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT3, AD9833DATA3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT4, AD9833DATA4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AD9833PORT, AD9833SCK, GPIO_PIN_SET); // Set All SPI pings to High
	HAL_GPIO_WritePin(AD9833PORT, AD9833SS, GPIO_PIN_SET);	// Set All SPI pings to High
	AD9833_SetWave(WaveType);								// Type Of Wave
	for (size_t i = 0; i < 4; i++)
	{
		myTransducer[i].frequency = myTransducer[i].frequency * 1000; // Khz//
		/* code */
	}

	AD9833_SetWaveData_Test(myTransducer); // Frequency & Phase Set
		for (size_t i = 0; i < 4; i++)
	{
		myTransducer[i].frequency = myTransducer[i].frequency / 1000; // Khz//
		/* code */
	}
	return;
}