/*
 * main.c
 */

/* Progetti di test per il bus I2C
	Created on: 31/gen/2015
 *
 *  tiva c
 *  I2C bus
 *  Author: robocupjr 15
 *
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "uartp/uartstdio.h"
#include "uartp/cmdline.h"
#include "tiva_i2c.h"
#include "gyro_init.h"
#include "accelInit.h"
#include "gen_def.h"
#include "gyro_f.h"
#include "uartp/uart.h"

#include "pid.h"


/// variabili globali
int procCom = 0;

int main(void) {
	
	volatile uint32_t valore = 0, i, blink = 0;
	volatile int32_t arrot;
	volatile int16_t val1 = 0, x, y, z;
	//uint8_t buffer[8];
	//volatile float f = 1.1098, e = (float)exp(1);
	//volatile double d = 1.9845637456;
	gyro G;
	pid C;


	/// disabilita le interruzioni
	DI();
	/// setup di base
	setupMCU();

    /// inizializza UART0
    ConfigureUART(115200, 0);
    //inizializzo l'i2c
	InitI2C0();
	/// messaggio d'inizio
	for(i = 0; i < 10000; i++);
	PRINTF("Modulo di test del bus I2C\n");
	for(i = 0; i < 10000; i++);
	PRINTF("Lettura dal sensore\n");
	valore = SysCtlClockGet();
	PRINTF("Frequenza del clock %d\n", valore);
	/// inizializza il giroscopio
	initGyro(&G, Z_AXIS);

	/// inizializza l'accelrometro
	//stato =  writeI2CByte(CTRL_REG1_A, ODR1 + ODR0 + ZaxEN + YaxEN + XaxEN);
	// scrivo nel registro 0x20 il valore 0x0F, cioe' banda minima, modulo on e assi on
	/// sintassi: indirizzo slave, num parm, indirizzo reg, valore da scrivere
	//I2CSend(ACCEL_ADDR, 2, CTRL_REG1_A, ODR1 + ODR0 + ZaxEN + YaxEN + XaxEN);

	/// inizializza il timer 0 e genera un tick da 10 ms
	initTimer0(10, &G);
	/// abilita le interruzioni
	EI();
	/// attende che il sensore vada a regime
	PRINTF("\nAzzeramento assi giroscopio\n");
	while (blink < 70){
		if (procCom == 1){
			procCom = 0;
			blink++;
		}
	}

	/// azzeramento degli assi
	azzeraAssi(&G);
	while(1){
		if (procCom == 1){
			HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_0 << 2))) |=  GPIO_PIN_0;
			PID(0, &G, &C);
			PWM(&C);
			procCom = 0;
			HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_0 << 2))) &=  ~GPIO_PIN_0;
			//PRINTF("asse x: %d\t", G.pitch);
			//PRINTF("\tasse y: %d\t", G.roll);
			PRINTF("\tasse z: %d\n", G.yaw);
			blink++;
			if (blink >= 100){
				HWREG(GPIO_PORTF_BASE + (GPIO_O_DATA + ((GPIO_PIN_2 | GPIO_PIN_1) << 2))) = 0;
				HWREG(GPIO_PORTF_BASE + (GPIO_O_DATA + (GPIO_PIN_3 << 2))) ^= GPIO_PIN_3;
				blink = 0;
			}
		}
		/*valore = I2CReceive(GYRO_ADDR,STATUS_REG);
		PRINTF("REG_STAT 0x%x\n", valore);
		if (valore != 0){
			/// legge i dati da tutti i registri del giroscopio
			//stato = readI2C_N_Byte(OUT_X_L_M, 6, buff);			/// compass
			I2CReceiveN(GYRO_ADDR,OUT_X_L | MUL_READ , 6, buffer);

			//valore = readI2CByteFromAddress(STATUS_REG, &stato);
			x = (int16_t)((buffer[1]<< 8) + buffer[0]) - G.x0;
			y = (int)((buffer[3]<< 8) + buffer[2]) - G.y0;
			z = (int)((buffer[5]<< 8) + buffer[4]) - G.z0;
			PRINTF("asse x: %d\t",x);
			PRINTF("\tasse y: %d\t",y);
			PRINTF("\tasse z: %i\r\n", z);
			//printf("contatore: %d\r\n", contatore);
			//i = 0;
			//contatore = 0;
			//P1OUT ^= 1;
			for (i = 0; i < 50000; i++);
	    }*/
	}
}
