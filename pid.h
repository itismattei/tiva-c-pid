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
	float 		kp;
	float 		kd;
	float 		ki;
	float 		e[2];		/// errori all'istante attuale e precedente
	float 		I;		///valore integrale
	float 		uscita;	/// valore dell'uscita
	uint32_t 	tipo;
} pid;

/// calcola l'aggiornamento del PID digital
void PID(int valFin, gyro *G, pid *C, pwm *P);
/// calcola il valore da fornire al PWM
void setPWM(pid *C, pwm *P);
/// imposta i coefficienti del PID
void setKpid(pid *C, float kp, float kd, float ki);

#endif /* PID_H_ */
