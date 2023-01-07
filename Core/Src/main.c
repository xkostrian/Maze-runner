/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "tim.h"
#include "gpio.h"
#include "hcsr04.h"
#include "PCR_servo.h"
#include <stdio.h>
#include <stdlib.h>

int k = 0, before = 0, state = 0;
int i1 = 0, i2 = 0, i3 = 0, i4 = 0;
float d1=1000, d2=1000, d3=1000, d4=1000;
int pomocna_premenna = 0, p2 = 0, p3 = 0;
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
int validate(int distance, int sensor, int count);
EDGE_TYPE edgeDetect(uint8_t pin_state, uint8_t samples);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Let's write the callback function

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
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);

  while (1)
  {
	  if (edgeDetect(!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5), 3) == RISE) {
		  if (state == 1) state = 0;
	  	  else state = 1;
	      switch (state) {
	      case 1:
	    	  PCR_go_forward();
	    	  break;
	  	  case 0:
	  		  PCR_stand_still();
	  		  break;
	  	  }
	  }
	   before = !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);

	   if (state == 1){
		   d3 = HCSR04_Read3();

		   if (d3 <= 15.0){
			   pomocna_premenna += 1;
		   } else{
			   pomocna_premenna = 0;
		   }

		   if (pomocna_premenna >= 5){
			   PCR_stand_still();
			   HAL_Delay(60);
			   d1 = HCSR04_Read1();
			   if (d1 <= 15.0){
				   p2 += 1;
			   } else{
				   p2 = 0;
			   }
			   p3 += 1;
			   if (p2 >= 4){
				   PCR_left_arc(0);
				   HAL_Delay(2000);
				   PCR_go_forward();
				   pomocna_premenna =0; p2 =0; p3=0;
			   } else if (p3 >= 5){
				   PCR_right_arc(0);
				   HAL_Delay(2000);
				   PCR_go_forward();
				   pomocna_premenna =0; p2 =0; p3=0;
			   }
			   HAL_Delay(60);
		   }
		   HAL_Delay(60);
	   }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
int validate(int distance, int sensor, int count){ //sensors_front=3, sensor_back=2, sensor_right=1, sensor_left=4
	float d = 0;
	int c = 0;
	for (int i=0; i<=count+1; i++){
		switch(sensor){
		case 1:
			d = HCSR04_Read1();
			HAL_Delay(60);
			break;
		case 2:
			d = HCSR04_Read2();
			HAL_Delay(60);
			break;
		case 3:
			d = HCSR04_Read3();
			HAL_Delay(60);
			break;
		case 4:
			d = HCSR04_Read4();
			HAL_Delay(60);
			break;
		}
		if (d <= (float)distance){
			c += 1;
		}
		if (c == count){
			return 1;
		}
	}
	return 0;
}

EDGE_TYPE edgeDetect(uint8_t pin_state, uint8_t samples){
	if (pin_state == 1 && before == 0 && k == 0){
		k = k+1;
	}
	if (pin_state == 1 && before == 1){
		 k = k+1;
	}
	if (k == samples){
		return RISE;
	}
	if (pin_state == 0){
		k = 0;
		before = 0;
		return NONE;
	}
	return NONE;
}

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
