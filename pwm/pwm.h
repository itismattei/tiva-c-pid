/*
 * pwm.h
 *
 *  Created on: 09/mar/2015
 *      Author: robotics
 */

#ifndef PWM_H_
#define PWM_H_


#define avanti 10
#define indietro 20
#define fermo 30

/*#define A 65	//avanti
#define I 73 	//indietro
#define S 83	//sinistra
#define D 68	//destra
#define F 70	//fermo
*/

typedef struct _pwm{

	uint32_t freq;
	uint32_t delta_1;
	uint32_t delta_2;
	uint8_t  dir_1;
	uint8_t dir_2;

} pwm;


void pwm_init(pwm *);
//void pwm_power(int delta_1, int delta_2, pwm*);
void pwm_power(pwm *);
void pwm_dir(pwm *);




#endif /* PWM_H_ */
