/*
 * gen_def.h
 *
 *  Created on: 31/gen/2015
 *      Author: massimo
 */

#ifndef GEN_DEF_H_
#define GEN_DEF_H_

#include "gyro_f.h"

#define		OFF				4
#define		ON				2
#define		OK				0
#define		NOT_PRESENT		3

#define		DI()	__asm(   "    cpsid   i\n")
#define		EI()	__asm(   "    cpsie   i\n")

void initTimer0(int interval, gyro *G);

#endif /* GEN_DEF_H_ */
