/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int _state = 0;
uint8_t tx_buff[] = "Basarili\r";
uint8_t rx_buff[100];
uint8_t oneByte;
uint8_t packet_data[100];
uint8_t _startIndex = 0;

uint8_t sizeOfpacket = 0;

_transducer myTransducer[4];
_transducer zeroTransducer[4];

unsigned long _period = 30;
unsigned long _duty = 15;
unsigned long _globalCounter = 0;

void parseData(const uint8_t *data, uint8_t size, _transducer *arrTransducer)
{
  uint8_t numIndex = 0;
  // uint8_t _numIndex = 0;
  uint8_t x;
  for (x = 0; x < size; x++)
  {
    if (*(data + x) == 'N')
    {
      arrTransducer[numIndex].number = *(data + x + 1) - 48;
    }
    if (*(data + x) == 'f')
    {
      uint8_t fValue[10] = {0};
      uint8_t frValueIndex = 0;
      uint8_t pValue[10] = {0};
      uint8_t pValueIndex = 0;
      x++;
      while (*(data + x) != 'p' && x < size)
      {
        fValue[frValueIndex] = *(data + x);
        frValueIndex++;
        x++;
      }
      x++;
      arrTransducer[numIndex].frequency = atoi(fValue);
      while (*(data + x) != 'N' && x < size && *(data + x) != 'F' && *(data + x) != 'T')
      {
        pValue[pValueIndex] = *(data + x);
        pValueIndex++;
        x++;
      }
      x--;
      arrTransducer[numIndex].phase = atoi(pValue);
      numIndex++;
    }
		else if(*(data + x) == 'T'){
			 x++; 
			 x++; //M
			 uint8_t period[10] = {0};
       uint8_t periodIndex = 0;
			 uint8_t duty[10] = {0};
       uint8_t dutyIndex = 0;
			while (*(data + x) != 'T' && x < size)
      {
        period[periodIndex] = *(data + x);
        periodIndex++;
        x++;
      }
			_period = atoi(period);
			x++;
			while (*(data + x) != 'F' && x < size)
      {
        duty[dutyIndex] = *(data + x);
        dutyIndex++;
        x++;
      }
			_duty = atoi(duty);
		}
  }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, &oneByte, 1);
  // HAL_UART_Transmit_IT(&huart1,tx_buff,sizeof(tx_buff)-1);
  // AD9833_Init(0,1000000,0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  for (size_t i = 0; i < 4; i++)
  {
    /* code */
    myTransducer[i].frequency = 1000000;
    myTransducer[i].phase = 0;
  }

  myTransducer[0].GPIOx = GPIOA;
  myTransducer[0].GPIO_Pin = GPIO_PIN_6;

  myTransducer[1].GPIOx = GPIOA;
  myTransducer[1].GPIO_Pin = GPIO_PIN_7;

  myTransducer[2].GPIOx = GPIOB;
  myTransducer[2].GPIO_Pin = GPIO_PIN_0;

  myTransducer[3].GPIOx = GPIOB;
  myTransducer[3].GPIO_Pin = GPIO_PIN_1;
	
	 for (size_t i = 0; i < 4; i++)
  {
    /* code */
    zeroTransducer[i].frequency = 0;
    zeroTransducer[i].phase = 0;
		zeroTransducer[i].GPIOx = myTransducer[i].GPIOx;
		zeroTransducer[i].GPIO_Pin = myTransducer[i].GPIO_Pin;
		zeroTransducer[i].number = 1+i;  
	}


  // AD9833_Init(0, 1000000, 1000000, 0, 0);
  AD9833_Init_Test(0, myTransducer);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    switch (_state)
    {
    case notReceived:
      __NOP();
      break;
    case packetReceived:
    {
      // memset(myTransducer, 0, sizeof(myTransducer));
      parseData(packet_data, sizeOfpacket, myTransducer);
      _state = sendAd9833;
      memset(packet_data, 0, sizeof(packet_data));
      sizeOfpacket = 0;
      break;
    }
    case sendAd9833:
			_globalCounter = 0;
      AD9833_Init_Test(0, myTransducer);
      char stringFrequency[10] = {0};
      char stringPhase[10] = {0};
      char sendData[30] = "STM-32[0]-> F:";
      char pString[] = " P:";
      sprintf(stringFrequency, "%d", myTransducer[0].frequency);
      sprintf(stringPhase, "%d", myTransducer[0].phase);
      strcat(sendData, stringFrequency);
      strcat(sendData, pString);
      strcat(sendData, stringPhase);
      //HAL_UART_Transmit_IT(&huart1, sendData, sizeof(sendData) - 1);
      _state = idleState;
      __NOP();
      break;
    case idleState:
			if(_duty >= _period){
			  __NOP();
				_globalCounter = 0;
			}
			else if(_globalCounter < _duty){
				HAL_Delay(1);
				_globalCounter++;
			}
			else if(_duty == _globalCounter){
				AD9833_Init_Test(0, zeroTransducer);
				HAL_Delay(1);
				_globalCounter++;
			}
			else if(_globalCounter < _period && _globalCounter > _duty){
				HAL_Delay(1);
				_globalCounter++;
				__NOP();
			}
			else if(_globalCounter == _period){
			  _globalCounter = 0;
				_state = sendAd9833;
			}
				
      break;
    default:
      __NOP();
      break;
    }
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, AD9833_SPI_CLK_Pin | AD9833_SPI_DATA_Pin | AD9833_SPI_SS_Pin | AD9833_SPI_DATA_Pin2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, AD9833_SPI_DATA_Pin3 | AD9833_SPI_DATA_Pin4, GPIO_PIN_RESET);
  /*Configure GPIO pins : AD9833_SPI_CLK_Pin AD9833_SPI_DATA_Pin AD9833_SPI_SS_Pin */
  GPIO_InitStruct.Pin = AD9833_SPI_CLK_Pin | AD9833_SPI_DATA_Pin | AD9833_SPI_SS_Pin | AD9833_SPI_DATA_Pin2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AD9833_SPI_CLK_Pin | AD9833_SPI_SS_Pin ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AD9833_SPI_DATA_Pin3 | AD9833_SPI_DATA_Pin4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  __NOP();
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  static uint8_t _index = 0;
  rx_buff[_index] = oneByte;
  if (_index >= 1)
  {
    if (rx_buff[_index] == 'C' && rx_buff[_index - 1] == 'P')
    {
      _startIndex = _index - 1;
    }
    if (rx_buff[_index] == 'F' && rx_buff[_index - 1] == 'F')
    {
      if (_state != packetReceived)
      {
        memcpy(packet_data, rx_buff, _index + 1 - _startIndex);
        sizeOfpacket = _index - _startIndex;
        _state = packetReceived;
      }
      memset(rx_buff, 0, sizeof(rx_buff));
      _startIndex = 0;
      _index = 0;
    }
  }
  if (_index > 98)
  {
    _index = 0;
    _state = errorPacket;
    memset(rx_buff, 0, sizeof(rx_buff));
  }
  _index++;
  memset(&oneByte, 0, 1);
  HAL_UART_Receive_IT(&huart1, &oneByte, 1);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
