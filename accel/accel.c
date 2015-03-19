/*
 * accel.c
 *
 *  Created on: 19/mar/2015
 *      Author: robotics
 */


#include "accel_init.h"


	/*          INIT ACCELEROMETER REGISTERS                */
		//initI2C_B1(FDCO, 800000, ACCEL_ADDR);
		//valore = readI2CByteFromAddress(CTRL_REG1_A, &stato);
		//stato =  writeI2CByte(CTRL_REG1_A, ODR1 + ODR0 + ZaxEN + YaxEN + XaxEN);

bool testAccel(){

	volatile uint32_t valore;

	/// legge il registro di controllo 1 che dovrebbe contenere 0x07
	valore = I2CReceive(ACCEL_ADDR, CTRL_REG1_A);
	if ((valore & 0xFF) == 7){
		PRINTF("Accelereometro OK! %d\n", valore);
		return true;
	}
	return false;
}

void impostaAccel(accelerazione *A){

	volatile uint32_t valore;
	/// imposta il campionamento a 50 sample / s ed abilita i 3 assi
	I2CSend(ACCEL_ADDR, 2, CTRL_REG1_A, ODR1 + ODR0 + ZaxEN + YaxEN + XaxEN);
	valore = I2CReceive(ACCEL_ADDR, CTRL_REG1_A);
}

void misuraAccelerazioni(accelerazione *A){

	volatile int x, y, z;
	uint8_t buffer[8];
	/// legge i valori degli assi
	if((I2CReceive(ACCEL_ADDR, STATUS_REG_A) & 0xF) != 0){
		I2CReceiveN(ACCEL_ADDR, OUT_X_L_A | MUL_READ , 6, buffer);
		x = (int16_t)((buffer[1]<< 8) + buffer[0]);
		y = (int16_t)((buffer[3]<< 8) + buffer[2]);
		z = (int16_t)((buffer[5]<< 8) + buffer[4]);
		A->v[0] = (float) x * 2 / 32768;
		A->v[1] = (float) y * 2 / 32768;
		A->v[2] = (float) z * 2 / 32768;
		PRINTF("acc x %d\t acc y %d\t acc z %d\n", x, y, z);
	}
}
