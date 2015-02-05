/*
 * gyro_f.c
 *
 *  Created on: 01/feb/2015
 *
 *  tiva c
 *  Inertial sensors
 *  Author: robocupjr 15
 */

#include "gyro_f.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/i2c.h"
#include "driverlib/rom.h"
#include "tiva_i2c.h"
#include "gyro_init.h"

///
/// integra la velocità rilevata dal sensore integrandola nell'intervallo di tempo.
void misuraAngoli(gyro *G){

	static uint32_t tempoDiReset = 0;
	uint32_t valore;
	int16_t z, x, y, tmp;
	uint8_t buffer[8];
	float f, DPS = G->FS;
	valore = I2CReceive(GYRO_ADDR, STATUS_REG);
	if (valore != 0){
		tempoDiReset++;
		/// legge i dati da tutti i registri del giroscopio
		/// stato = readI2C_N_Byte(OUT_X_L_M, 6, buff);			/// compass
		if ((G->asseOn & Z_AXIS) == Z_AXIS){
			I2CReceiveN(GYRO_ADDR, OUT_Z_L | MUL_READ , 2, buffer);
			z = (int16_t)((buffer[1]<< 8) + buffer[0]) - G->z0;
			/// integrazione rettangolare: valore letto * fondo scala * intervallo di tempo di integrazione
			/// posto a 10ms
			f = z * DPS;
			f *= G->tick;
			G->yawF += f;

			if (tempoDiReset >= 1000){
				/// sono passati 5 secondi e dovrebbe ricalcolare l'offset degli assi
				/// dovrebbe calcolare se G->yawF è cambiato di almeno 1 grado rispetto ai 5 secondi precedenti
				/// se non è cmbiato dovrebbe azzerare la parte frazionaria
				if ((G->yawF - G->yawP < 1.0) && (G->yawF - G->yawP > -1.0)){
					/// dopo 5 secondi non ho avuto variazioni significative e quindi ho integrato l'errore del
					/// sensore
					G->yawF = G->yawP;
				}
				else{
					tmp = (int16_t)G->yawF;
					G->yawP = (float) tmp;
				}
				tempoDiReset = 0;
			}
			/// riporta il valore ad intero
			G->yaw = (int16_t) G->yawF;
		}

		if((G->asseOn & ALL_AXIS) == ALL_AXIS){
			I2CReceiveN(GYRO_ADDR, OUT_X_L | MUL_READ , 6, buffer);
			x = (int16_t)((buffer[1]<< 8) + buffer[0]) - G->x0;
			y = (int)((buffer[3]<< 8) + buffer[2]) - G->y0;
			z = (int)((buffer[5]<< 8) + buffer[4]) - G->z0;
			/// integrazione rettangolare: valore letto * fondo scala * intervallo di tempo di integrazione
			/// posto a 10ms
			f = z * DPS;
			G->yawF = f;
			f *= G->tick;
			/// riporta il valore ad intero
			G->yaw += (int16_t) f;
			f = y * DPS;
			G->rollF = f;
			f *= G->tick;
			/// riporta il valore ad intero
			G->roll += (int16_t)f;
			f = x * DPS;
			G->pitchF = f;
			f *= G->tick;
			/// riporta il valore ad intero
			G->pitch += (int16_t) f;
		//valore = readI2CByteFromAddress(STATUS_REG, &stato);

		/*PRINTF("asse x: %d\t",x);
		PRINTF("\tasse y: %d\t",y);
		PRINTF("\tasse z: %i\r\n", z);*/
		}
	}
	else{
		/// integra dal valore precedente con intervallo di tempo di 10ms
		f = G->yawF * G->tick;
		G->yaw += (uint16_t) f;
		if ((G->asseOn & 0x3) != 0){
			/// occorre integrare anche sugli assi X e Y
			f = G->rollF * G->tick;
			G->roll += (uint16_t) f;
			f = G->pitchF * G->tick;
			G->pitch += (uint16_t) f;
		}
	}
}

