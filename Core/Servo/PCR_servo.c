/*
 * PCR_servo.c
 *
 *  Created on: 13. 12. 2022
 *      Author: CizmarJ
 */

#include "PCR_servo.h"
#include <stdio.h>
#include <stdlib.h>

int random_number = 0;

void PCR_stand_still(){

	VL = 1500;
	VR = 1500;
	}

void PCR_go_forward(){

	random_number = 1;//rand() % 3 + 1;
	switch (random_number){
	case 1:
		VL = 1600;
		VR = 1400;
		break;
	case 2:
		VL = 1600;
		VR = 1460;
		break;
	case 3:
		VL = 1540;
		VR = 1400;
		break;
	default:
		VL = 1600;
		VR = 1400;
		break;
	}

}

void PCR_go_backwards(){

	VL = 1400;
	VR = 1600;
}

void PCR_set_speeds(int left, int right){

	if(left < -100){
		left = -100;
	}
	if(left > 100){
		left = 100;
	}
	if(right < -100){
		right = -100;
	}
	if(right > 100){
		right = 100;
	}
	VL = 1500 + left;
	VR = 1500 - right;
}

void PCR_right_arc(int mode){
	switch(mode){
		case 0:
			VL = 1600;
			VR = 1500;
			break;
		case 1:
			VL = 1600;
			VR = 1480;
			break;
		case 2:
			VL = 1600;
			VR = 1460;
			break;
		case 3:
			VL = 1600;
			VR = 1440;
			break;
		default:
			VL = 1600;
			VR = 1400;
			break;
	}
}

void PCR_left_arc(int mode){
	switch(mode){
			case 0:
				VL = 1500;
				VR = 1400;
				break;
			case 1:
				VL = 1520;
				VR = 1400;
				break;
			case 2:
				VL = 1540;
				VR = 1400;
				break;
			case 3:
				VL = 1560;
				VR = 1400;
				break;
			default:
				VL = 1600;
				VR = 1400;
				break;
		}
}




