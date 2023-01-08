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
int p1 = 0, p2 = 0, p3 = 0, jp1 = 0, j = 0;
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
int validate(int distance, float *sensor, int *current_status, int count);
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
		   j += 1;
		   if (j > 5){
			   j = 0;
			   if (jp1 < 5 && HCSR04_Read2() <= 20){
				   p1 = 0;
				   HAL_Delay(60);
				   if (HCSR04_Read3() > 20){
					   PCR_go_forward();
				   }
				   HAL_Delay(60);
			   } else if (jp1 < 5){
				   p1 = 0;
				   HAL_Delay(60);
				   PCR_go_forward();
			   }
		   }

		   d3 = HCSR04_Read3();

		   if (validate(25, &d3, &i3, 5) == 1){ //front
			   PCR_stand_still();
			   HAL_Delay(60);
			   d1 = HCSR04_Read1();
			   if (validate(20, &d1, &i1, 5) == 1){ //right
				   PCR_stand_still();
				   HAL_Delay(60);
				   d4 = HCSR04_Read4();
				   if (validate(20, &d4, &i4, 5) == 1){ //left
					   PCR_stand_still();
					   HAL_Delay(60);
					   d2 = HCSR04_Read2();
					   if (validate(20, &d2, &i2, 5) == 1){ //back
						   PCR_stand_still();
					   } else{
						   p3 += 1;
					   }
					   if (p3 >= 5){
						   PCR_go_backwards();
						   HAL_Delay(2000);
						   PCR_right_arc(0);
						   HAL_Delay(2000);
						   PCR_go_forward();
						   i1 = 0; i3 = 0; p1 = 0; p2 = 0; p3 = 0; i2 = 0; i4 = 0;
					   } //back end
				   } else{
					   p2 += 1;
				   }
				   if (p2 >= 5){
					   PCR_left_arc(0);
					   HAL_Delay(2000);
					   PCR_go_forward();
					   i1 = 0; i3 = 0; p1 = 0; p2 = 0; p3 = 0; i2 = 0; i4 = 0;
				   } //left end
			   } else{
				   p1 += 1;
			   }
			   if (p1 >= 5){
				   PCR_right_arc(0);
				   HAL_Delay(2000);
				   PCR_go_forward();
				   i1 = 0; i3 = 0; p1 = 0; p2 = 0; p3 = 0; i2 = 0; i4 = 0;
			   } //right end
		   } //front end
		   jp1 = p1;
		   HAL_Delay(60);

		   if (i3 == 0){
			   d1 = HCSR04_Read1();
			   if (validate(10, &d1, &i1, 2) == 1){ //right II
				   PCR_left_arc(1);
			   }
			   HAL_Delay(60);

			   d4 = HCSR04_Read4();
			   if (validate(10, &d4, &i4, 2) == 1){ //left II
				   PCR_right_arc(1);
			   }
			   HAL_Delay(60);
		   }
	   }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
int validate(int distance, float *sensor, int *current_status, int count){ //sensors_front=3, sensor_back=2, sensor_right=1, sensor_left=4
	if (*sensor <= distance){
		*current_status += 1;
	} else{
		*current_status = 0;
	}
	if (*current_status >= count-1){
//		*current_status = 0;
		return 1;
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
