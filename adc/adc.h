/*
 * adc.h
 *
 *  Created on: 15/mar/2015
 *      Author: robotics
 */

#ifndef ADC_H_
#define ADC_H_

///
/// struttura che mantiene la distanza misurata dai 5 sensori
/// e la conversione in millimetri
typedef struct _dist{

	float 		misSens[5];
	int 		d_mm[5];

} distanza;

void initAdc(distanza *);



#endif /* ADC_H_ */
