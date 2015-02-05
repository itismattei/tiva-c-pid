/*
 * pid.c
 *
 *  Created on: 05/feb/2015
 *  PID
 *  Author: robocupjr 15
 */

#include <math.h>
#include "pid.h"

void PID(int valFin, gyro *G, pid *C){
	float D, P;
	///provvede ad integrare la misura della velcita' angolare
	misuraAngoli(G);
	C->e[1] = (float) (valFin - G->yaw);
	///void pwm_motor(int delta_1, int delta_2)
	D = C->kd * (C->e[1] - C->e[0]) / G->tick;
	P = C->kp * C->e[1];
	C->I += C->ki * (C->e[1] + C->e[0])/2 * G->tick;
	C->uscita = D + P + C->I;
	/// dispositivo con saturazione
	if (C->uscita > 100.0)
		C->uscita = 100.0;
		else if (C->uscita < -100.0)
			C->uscita = -100.0;
	//aggiornamento dell'uscita
	C->e[0] = C->e[1];

}

void PWM(pid *C){

	/// la rotazione antioraria e' positiva



}
