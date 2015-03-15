/*
 * adc.c
 *
 *  Created on: 15/mar/2015
 *      Author: robotics
 */

#include "adc.h"

void initAdc(distanza *d){
	int i;
	for (i = 0; i < 5; i++){
		d->misSens[i] = 0.0;
		d->d_mm[i] = 0;
	}

}

