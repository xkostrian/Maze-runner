#ifndef HCSR04_H_
#define HCSR04_H_

#include "main.h"
#include "tim.h"

#define TRIG_PIN GPIO_PIN_9
#define TRIG_PORT GPIOA

void delay(uint16_t time);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
float HCSR04_Read(void);
float HCSR04_Read3(void);

#endif /* HCSR04_H_ */
