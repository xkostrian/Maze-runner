/*
 * PCR_servo.h
 *
 *  Created on: 13. 12. 2022
 *      Author: CizmarJ
 */

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include <math.h>

#define VR  	TIM3 -> CCR1
#define VL  	TIM3 -> CCR2
#define L	 	0.12
#define R 	 	0.065;

void PCR_stand_still();
void PCR_go_forward();
void PCR_set_speeds(int left, int right);

void PCR_left_arc(int mode);
void PCR_right_arc(int mode);
