#include "hcsr04.h"

uint32_t IC_Val1a, IC_Val1b, IC_Val2a, IC_Val2b, IC_Val3a, IC_Val3b, IC_Val4a, IC_Val4b = 0;
float Difference1, Difference2, Difference3, Difference4 = 0;
uint8_t Is_First_Captured1, Is_First_Captured2, Is_First_Captured3, Is_First_Captured4 = 0;  // is the first value captured ?
float Distance1, Distance2, Distance3, Distance4  = 0;

void delay(uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (__HAL_TIM_GET_COUNTER (&htim2) < time);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // if the interrupt source is channel1
	{
		if (Is_First_Captured1==0) // if the first value is not captured
		{
			IC_Val1a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
			Is_First_Captured1 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (Is_First_Captured1==1)   // if the first is already captured
		{
			IC_Val1b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val1b > IC_Val1a)
			{
				Difference1 = IC_Val1b-IC_Val1a;
			}
			else if (IC_Val1a > IC_Val1b)
			{
				Difference1 = (0xffff - IC_Val1a) + IC_Val1b;
			}

			Distance1 = Difference1 * .034/2 * 10; //cm
			Is_First_Captured1 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if (Is_First_Captured2==0) // if the first value is not captured
		{
			IC_Val2a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
			Is_First_Captured2 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (Is_First_Captured2==1)   // if the first is already captured
		{
			IC_Val2b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val2b > IC_Val2a)
			{
				Difference2 = IC_Val2b-IC_Val2a;
			}
			else if (IC_Val2a > IC_Val2b)
			{
				Difference2 = (0xffff - IC_Val2a) + IC_Val2b;
			}

			Distance2 = Difference2 * .034/2 * 10; //cm
			Is_First_Captured2 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC2);
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if (Is_First_Captured3==0) // if the first value is not captured
		{
			IC_Val3a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3); // read the first value
			Is_First_Captured3 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (Is_First_Captured3==1)   // if the first is already captured
		{
			IC_Val3b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val3b > IC_Val3a)
			{
				Difference3 = IC_Val3b-IC_Val3a;
			}
			else if (IC_Val3a > IC_Val3b)
			{
				Difference3 = (0xffff - IC_Val3a) + IC_Val3b;
			}

			Distance3 = Difference3 * .034/2 * 10; //cm
			Is_First_Captured3 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC3);
		}
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if (Is_First_Captured4==0) // if the first value is not captured
		{
			IC_Val4a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4); // read the first value
			Is_First_Captured4 = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (Is_First_Captured4==1)   // if the first is already captured
		{
			IC_Val4b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (IC_Val4b > IC_Val4a)
			{
				Difference4 = IC_Val4b-IC_Val4a;
			}
			else if (IC_Val4a > IC_Val4b)
			{
				Difference4 = (0xffff - IC_Val4a) + IC_Val4b;
			}

			Distance4 = Difference4 * .034/2 * 10; //cm
			Is_First_Captured4 = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC4);
		}
	}
}

float HCSR04_Read1(void) //right
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC1);

	return Distance1;
}

float HCSR04_Read2(void) //back
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC2);

	return Distance2;
}

float HCSR04_Read3(void) //front
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC3);

	return Distance3;
}

float HCSR04_Read4(void) //left
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_CC4);

	return Distance4;
}
