/*
 * pid.h
 *
 *  Created on: 05/feb/2015
 *  PID
 *  Author: robocupjr 15
 */

#ifndef PID_H_
#define PID_H_

#include "gyro_f.h"


typedef struct _pid{
	float kp;
	float kd;
	float ki;
	float e[2];		/// errori all'istante attuale e precedente
	float I;		///valore integrale
	float uscita;	/// valore dell'uscita
} pid;

void PID(int valFin, gyro *G, pid *C);
void PWM(pid *C);


#endif /* PID_H_ */
