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
#include "pwm/pwm.h"


typedef struct _pid{
	float kp;
	float kd;
	float ki;
	float e[2];		/// errori all'istante attuale e precedente
	float I;		///valore integrale
	float uscita;	/// valore dell'uscita
} pid;

/// calcola l'aggiornamento del PID digital
void PID(int valFin, gyro *G, pid *C);
/// calcola il valore da fornire al PWM
void setPWM(pid *C, pwm *P);


#endif /* PID_H_ */
