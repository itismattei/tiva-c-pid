/*
 * gyro_f.h
 *
 *  tiva c
 *  Inertial sensors
 *  Author: robocupjr 15
 */

#ifndef GYRO_F_H_
#define GYRO_F_H_

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef struct _gyro{
	char IsPresent;
	char IsOn;
	/// tiene il setup dell'asse
	int16_t x0, y0, z0;
	/// indica quali assi sono attivi
	char asseOn;
	/// valore degli angoli misurati da integrazione della rotazione
	/// i valori sono in gradi in modo da non dover memorizzare dei float
	int16_t yaw; 	/// azze z
	int16_t roll; 	/// asse y
	int16_t pitch;	/// asse x

	/// valori attuali degli assi
	float yawF;
	float rollF;
	float pitchF;
	/// valori precedenti degli assi
	float yawP, rollP, pitchP;
	/// valori di gradi al secondo dedotti dal manuale del giroscopio
	int16_t gradiSec;
	float FS;
	/// intervallo di integrazione in ms
	float tick;
	/// fattori di scala per ciscun asse
	float kz, ky, kx;
} gyro;

/// fondo scala gradi per secondo / 2^(16-1)
#define		DPS_500		0.01525879
#define		DPS_250		0.00762939

void misuraAngoli(gyro *G);

#endif /* GYRO_F_H_ */
