/*
 * pid.c
 *
 *  Created on: 05/feb/2015
 *  PID
 *  Author: robocupjr 15
 */

#include <math.h>
#include "pid.h"

void PID(int valFin, gyro *G){
	///provvede ad integrare la misura della velcita' angolare
	misuraAngoli(G);
}

