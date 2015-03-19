/*
 * accel_init.h
 *
 *  Created on: 30/dic/2013
 *      Author: robotics
 */

#ifndef ACCEL_INIT_H_
#define ACCEL_INIT_H_

///////////////////////////////////////////////
///// ADDRESS
///////////////////////////////////////////////
#define		ACCEL_ADDR		0x19


#define		CTRL_REG1_A		0x20
#define		CTRL_REG2_A		0x21
#define		CTRL_REG3_A		0x22
#define		CTRL_REG4_A		0x23
#define		CTRL_REG5_A		0x24
#define		CTRL_REG6_A		0x25
#define		STATUS_REG_A	0x27

#define		OUT_X_L_A 		0x28
#define		OUT_X_H_A 		0x29
#define		OUT_Y_L_A 		0x2A
#define		OUT_Y_H_A 		0x2B
#define		OUT_Z_L_A 		0x2C
#define		OUT_Z_H_A 		0x2D

#define		FIFO_CTRL_REG_A 0x2E



///////////////////////////////////////////////
////// BIT
///////////////////////////////////////////////
#define		ODR3			0x80
#define		ODR2			0x40
#define		ODR1			0x20
#define		ODR0			0x10
#define		LOW_POW_EN		0x08
#define		ZaxEN			0x04
#define		YaxEN			0x02
#define		XaxEN			0x01


#define		FS0				16
#define		FS1				32




#endif /* ACCEL_INIT_H_ */
