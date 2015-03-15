/*
 * pid.c
 *
 *  Created on: 05/feb/2015
 *  PID
 *  Author: robocupjr 15
 */

#include <math.h>
#include "pid.h"
#include "pwm\pwm.h"


void setKpid(pid *C, float kp, float kd, float ki){
	C->kp = kp;
	C->kd = kd;
	C->ki = ki;
}

void PID(int valFin, gyro *G, pid *C, pwm *PWM){
	float D, P, I;
	///provvede ad integrare la misura della velcita' angolare
	misuraAngoli(G);
	C->e[1] = (float) (valFin - G->yaw);
	///void pwm_motor(int delta_1, int delta_2)
	D = C->kd * (C->e[1] - C->e[0]) / G->tick;
	P = C->kp * C->e[1];
	I = C->I + C->ki * G->tick * (C->e[1] + C->e[0]);
	I *= (float)0.50;
	C->I = I;
	C->uscita = D + P + C->I;
	/// dispositivo con saturazione
	if (C->uscita > 100.0)
		C->uscita = 100.0;
		else if (C->uscita < -100.0)
			C->uscita = -100.0;
	//aggiornamento dell'uscita
	C->e[0] = C->e[1];
	if (C->tipo == 0){

		/// avanti oppure indietro
		PWM->delta_1 = (uint32_t) C->uscita * 0.95;
		PWM->delta_2 =PWM->delta_1;
	}
	else
		if (C->tipo == 1){
		/// rotazione soft
		//TODO: DA FARE
		}
		else {
			/// rotazione brusca
		}


}

void setPWM(pid *C, pwm *PWM){


}
