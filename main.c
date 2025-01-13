/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
 
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hx711.h"
#include "LCD_16x2.h"
#include <stdio.h>
#include <string.h>
 
 
#define hx711_delay(x)    HAL_Delay(x)
extern TIM_HandleTypeDef htim1;

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
 
/* USER CODE BEGIN PV */

/*Global Variables*/
hx711_t loadcell;
float weight;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);

/*User Defined Functions*/
void Crazy_Comments(void);
void hx711_calibration_process(void);
void Necessary_init(void);
void GPIO_init(void);
 
 
int main(void)
{
	Necessary_init();
	lcd_print("Measurement and Instrumentation CEP");
	HAL_Delay(1000);
	lcd_sendCommand(0x01);
	lcd_print("Weight Machine");	
	hx711_init(&loadcell, GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_2);
	hx711_calibration_process();
	Crazy_Comments();
	float plate_weight = 0.45;
  while (1)
  {		
		lcd_print("Weight Machine");
    weight = hx711_weight(&loadcell, 10);
		float object_weight = weight-plate_weight;
		HAL_Delay(10);
		lcd_sendCommand(0xC0);
		lcd_print("Weight:");
		lcd_printFloat(object_weight,2);
		lcd_print("kg");
    HAL_Delay(2000);
		lcd_sendCommand(0x01);
  }
}

void Necessary_init(void)
{
	HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM1_Init();
  GPIO_init();
	lcd_init();
}

void hx711_calibration_process(void)
{
	
	/*
		int64_t load_raw =0;
		// The following piece of code is to be used for computing the averaged value of hx711() for Zero Weight on Machine and a Known Weight on the machine
		for (int i = 0; i <10;++i)
		{
			load_raw = load_raw + hx711_value(&loadcell);
			hx711_delay(5);
		}
		//The known weight ADC value from hx711() is displayed on 16x2 LCD for calculating the callibration constant
		int32_t answer = (int32_t)(load_raw/10);
		lcd_sendCommand(0x01);
		lcd_print("Known Weight ADC_val:");
		lcd_sendCommand(0xC0);
		lcd_printInt(answer);
		HAL_Delay(3000);
		lcd_sendCommand(0x01);
		*/
	
	
	int32_t tare    = 8334130;// Determined after the implementing the above logic and visualizing on LCD
	int32_t load_raw = 8519335;// Determined after the implementing the above logic by placing a weight of 8kg and visualizing on LCD	
	
	//To perform new callibration modify the loadraw value by the corresponding weight.Tare value is constant for every callibration
	
		/*Note: To obtain the Tare value just replace the variable load_raw to tare and modify the string and value to be displayed on LCD*/

		// After obtaining the Tare and Known_weight_ADC value we can call the callibration function which will compute the callibration coefficent
		//hx711_calibration(&loadcell,tare,load_raw,8);
	
	
	hx711_coef_set(&loadcell,23150); // 23150 is obtained by (load_raw-tare)/8 that is going to be fix for every weight measurement
}


void GPIO_init(void)
{
		RCC->APB2ENR |= 0xFD; /* Enable clocks for GPIO ports + AFIO */
		GPIOB->CRH    = 0x33334444;/* D4-D7 on PB12-PB15 as GPO @50MHz*/
		GPIOA->CRH    = 0x44433444;/* rs and en on PB11-PB12 respectively as GPO @50MHz*/

}


void Crazy_Comments(void)
{
  
	//hx711_tare(&loadcell, 10);
	//lcd_printInt()
	//lcd_print("place wight");
	//HAL_Delay(5000);

// Measure raw ADC value with 4 kg weight
//int32_t load_raw = 7728079;
//int32_t load_raw = hx711_value_ave(&loadcell, 10);
//int32_t tare = 8377867;
//int32_t tare = 8388607;//
//int32_t load_raw = 7848347;
//int32_t load_raw = 7183201;//
//int32_t load_raw = 8672167;//


//hx711_coef_set(&loadcell, 178686); 
//hx711_coef_set(&loadcell, 873747); 
//hx711_coef_set(&loadcell, 35250); 


// Perform calibration (4 kg weight)


	//hx711_coef_set(&loadcell, 137447); // read afer calibration  36057.14
  //hx711_coef_set(&loadcell, 107380);
	//hx711_coef_set(&loadcell, 301351);
	//hx711_coef_set(&loadcell, 165380);
	
  /* USER CODE END 2 */
 
  /* USER CODE BEGIN WHILE */

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
