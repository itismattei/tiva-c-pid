/*
 * pwm.c
 *
 *  Created on: 09/mar/2015
 *      Author: robotics
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "pwm.h"



#define IN1 	GPIO_PIN_6
#define IN2 	GPIO_PIN_7
#define IN3 	GPIO_PIN_3
#define IN4 	GPIO_PIN_2


///in1, in2 -> pb6, pb7
///in3, in4 -> pa3, pa2

///
/// direzione del mezzo
void pwm_dir(pwm *p){
	/*  in1 = in3 = 1; in2 = in4 = 0  avanti
	 *  in1 = in3 = 0; in2 = in4 = 1  indietro
	 *  in1 = 1, in2 = 0; in3 = 0, in4 = 1 rotazione antioraria
	 *  in1 = 0, in2 = 1; in3 = 1, in4 = 0 rotazione oraria
	 *
	 *  */
	uint8_t direzione = ((p->dir_1 & 0xFF) << 4) | (p->dir_2 & 0xFF);
	switch(direzione){
	case 0x11:
		//avanti: IN1A = 1, IN2A = 1
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN4) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN2) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN1) << 2)) |=  IN1;
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN3) << 2)) |=  IN3;
	break;

	case 0x22:
		// indietro: IN1B = 1, IN2B = 1
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN3) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN1) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN2) << 2)) |=  IN2;
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN4) << 2)) |=  IN4;
	break;

	case 0x21:
		// rotazione antioraria: IN1B = 1, IN2B = 1
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN3) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN1) << 2)) |=  IN1;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN2) << 2)) =  0;
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN4) << 2)) |=  IN4;
	break;

	case 0x12:
		// rotazione oraria
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN4) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN2) << 2)) |=  IN2;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN1) << 2)) =  0;
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN3) << 2)) |=  IN3;

	break;

	case 0x00:
	default:
		// stop
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN4) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN2) << 2)) =  0;
		HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (IN1) << 2)) =  0;
		HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (IN3) << 2)) =  0;
	break;
	}
}


//*****************************************************************************
//
// Funzione per mandare avanti i motori
//
//*****************************************************************************
//void pwm_power(int delta_1, int delta_2, pwm *p)
void pwm_power(pwm *p){

	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, p->freq * p->delta_1 / 100);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, p->freq * p->delta_2 / 100);

}


void pwm_init(pwm *p){

	/// Abilita le porte usate dal PWM, e per il comando della direzione dei motori,
	/// anche se potrebbero gia' esserlo stato inaltre parti del programma
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	///in1, in2 -> pb6, pb7
	///in3, in4 -> pa3, pa2
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, IN1 | IN2);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, IN3 | IN4);
	/// azzeramento delle uscite dei pin
	HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_6 | GPIO_PIN_7) << 2)) =  0;
	HWREG(GPIO_PORTA_BASE + (GPIO_O_DATA + (GPIO_PIN_3 | GPIO_PIN_2) << 2)) =  0;
    //
    // Enable the GPIO port B
    //
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);


	int ulPeriod;
    ulPeriod = SysCtlClockGet() / 16000; //PWM frequency 400HZ
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_2);  //divisore per 2
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); //modulo pwm numero 0

    //Configure PB4, PB5 Pins as PWM
    ROM_GPIOPinConfigure(GPIO_PB5_M0PWM3);  //generatore 3
    ROM_GPIOPinConfigure(GPIO_PB4_M0PWM2); // generatore 4
    ///PB4 e' il modulo 1, mentre PB5 e' il modulo 2 del L293E
    ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);


    //il generatore dovrebbe essere il numero 1
    //TODO: verificare che sia vero
    ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);


    //Set the Period (expressed in clock ticks)
    ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ulPeriod);


    //Set PWM duty-50% (Period /2)
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,0);
    ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,0);




    // Enable the PWM generator
    ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    // Turn on the Output pins
    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT|PWM_OUT_2_BIT, true);

    p->freq = SysCtlClockGet() / 16000;
    p->delta_1 = 0;
    p->delta_2 = 0;
    pwm_power(p);

}
