/*
 * gyro_init.h
 *
 *
 *  Created on: 31/gen/2015
 *
 *  tiva c
 *  Inertial sensors
 *  Author: robocupjr 15
 */

#include "gyro_f.h"

#ifndef GYRO_INIT_H_
#define GYRO_INIT_H_

///////////////////////////////////////////////
///// ADDRESS
///////////////////////////////////////////////
#define		GYRO_ADDR		0x6B


///////////////////////////////////////////////
///
///  		GYROSCOPE REGISTERS ADDRESSES
///
///////////////////////////////////////////////

#define		WHO_AM_I		0x0F
#define		CTRL_REG1		0x20
#define		CTRL_REG2		0x21
#define		CTRL_REG3		0x22
#define		CTRL_REG4		0x23
#define		CTRL_REG5		0x24

#define		OUT_TEMP		0x26
#define		STATUS_REG		0x27
/////
/////  DATA REGISTER
#define		OUT_X_L 		0x28
#define		OUT_X_H 		0x29
#define		OUT_Y_L 		0x2A
#define		OUT_Y_H 		0x2B
#define		OUT_Z_L 		0x2C
#define		OUT_Z_H 		0x2D

////
//// fifo  register
#define		FIFO_CTRL_REG 	0x2E
#define		FIFO_SRC_REG 	0x2F

////
//// USEFULL BITS

#define		ZYXOR 			0x80
#define		ZOR 			0x40
#define		YOR 			0x20
#define		XOR 			0x10
#define		ZYXDA 			0x08
#define		ZDA 			0x04
#define		YDA 			0x02
#define		XDA				0x01

/*0 0 0 Bypass mode
0 0 1 FIFO mode
0 1 0 Stream mode
0 1 1 Stream-to-FIFO mode
1 0 0 Bypass-to-Stream mode*/
#define 	BYPASS_MODE			0x00
#define		FIFO_MODE			32
#define		STREAM_MODE			64
#define		STREAM_TO_FIFO		96
#define		BYPASS_TO_STREAM	128
#define		WTM0				1
#define		WTM1				2
#define		WTM2				4
#define		WTM3				8
#define		WTM4				16

//I1_Int1 I1_Boot H_Lactive PP_OD I2_DRDY I2_WTM I2_ORun I2_Empty

#define 	I1_INT1				128
#define 	I1_BOOT				64
#define 	H_LACTIVE			32
#define 	PP_OD				16
#define		I2_DRDY				8
#define		I2_WTM				4
#define		I2_ORUN				2
#define		I2_EMPTY			1



#define		FS0					16
#define		FS1					32
#define		FS_250				0
#define		FS_500				16
#define		FS_2000				32

#define		FIFO_EN				64
#define 	MUL_READ			0x80

#define		ALL_AXIS			0x7
#define		Z_AXIS				0x4




void initGyro(gyro *, char);
/// imposta quali assi saranno ON
void setupAssi(gyro *, char);
/// calcola la media degli assi e imposta l'offset
void azzeraAssi(gyro *);
/// set up di base: clock, led, ingresso di avvio
void setupMCU();
void attesa(int);


#endif /* GYRO_INIT_H_ */
