/*
 * gyro_init.c
 *
 *  tiva c
 *  Inertial sensors
 *  Author: robocupjr 15
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "gyro_init.h"
#include "uartp/uartstdio.h"
#include "tiva_i2c.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "gen_def.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"


void initGyro(gyro *G, char assi){
	volatile uint32_t valore;
	G->IsOn = OFF;
	G->yaw = G->roll = G->pitch = 0;
	//chiedo il dato presente nel registro WHO_AM_I
	if (I2CReceive(GYRO_ADDR, WHO_AM_I) == 0xD4){
		GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED, BLUE_LED);
		G->IsPresent = OK;
		/// imposta gli assi
		/// per il drone, stato = ALL (0x7)
		/// per il rover stato = Z (0x4);
		setupAssi(G, assi);
	}
	else{
		G->IsPresent = NOT_PRESENT;
		PRINTF("Giroscopio non presente\n");
		return;
	}
}

///
/// imposta gli assi che dovranno essere attivati e memorizza lo stato
void setupAssi(gyro *G, char stato){

	char mask;
	uint32_t valore;
	G->x0 = G->y0 = G->z0 = 32767;
	G->pitchF = G->rollF = G->yawF = G->pitchP = G->rollP = G->yawP = 0.0;
	G->gradiSec = 0;
	G->kz = 1.1688;
	/// lo stato e' cosi' interpretato: bit0: x; bit1: y; bit2: z.
	// scrivo nel registro 0x20 il valore 0x0C, cioe' banda minima, modulo on e assi on
	/// sintassi: indirizzo slave, num parm, indirizzo reg, valore da scrivere
	mask = 0x08| stato;
	I2CSend(GYRO_ADDR, 2, CTRL_REG1, mask);
	if(I2CReceive(GYRO_ADDR, CTRL_REG1) == mask){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
		G->IsOn = ON;
		G->asseOn = stato;
	}

	/// set FS to 500 degree per sec.
	I2CSend(GYRO_ADDR, 2, CTRL_REG4, FS_500);
	G->gradiSec = 500;
	G->FS = (float) 500 / 32768;
	valore = I2CReceive(GYRO_ADDR,CTRL_REG4);
	PRINTF("Lettura dal REG4 %d\n", valore);
}


/// calcola la media degli assi e imposta l'offset
void azzeraAssi(gyro *G){
	uint32_t conteggio = 0, valore, i ;
	int16_t x = 0, y = 0, z = 0;
	uint8_t buffer[8];
	switch(G->asseOn){
	case ALL_AXIS:
		/// assi ON
		while(conteggio < 32){
			/// effettua 32 letture e calcola la media
			valore = I2CReceive(GYRO_ADDR,STATUS_REG);
			//PRINTF("REG_STAT 0x%x\n", valore);
			if (valore != 0){
				/// tutti gli assi on
				I2CReceiveN(GYRO_ADDR,OUT_X_L | MUL_READ , 6, buffer);
				x += (int16_t)((buffer[1]<< 8) + buffer[0]);
				y += (int)((buffer[3]<< 8) + buffer[2]);
				z += (int)((buffer[5]<< 8) + buffer[4]);
				conteggio++;
				for (i = 0; i < 50000; i++);
			}
		}
		/// calcola la media: divide  per 32
		x /= 32;
		y /= 32;
		z /= 32;
		/// assegna il valore nella struct G
		G->x0 = x;
		G->y0 = y;
		G->z0 = z;
	break;

	case Z_AXIS:
		/// asse z ON
		while(conteggio < 32){
			/// effettua 32 letture e calcola la media
			valore = I2CReceive(GYRO_ADDR,STATUS_REG);
			//PRINTF("REG_STAT 0x%x\n", valore);
			if (valore != 0){
				/// asse z ON
				I2CReceiveN(GYRO_ADDR,OUT_Z_L | MUL_READ , 2, buffer);
				z += (int16_t)((buffer[1]<< 8) + buffer[0]);
				conteggio++;
				for (i = 0; i < 50000; i++);
			}
		}
		/// calcola la media: divide  per 32
		z /= 32;
		/// assegna il valore nella struct G
		G->z0 = z;
	break;
	}
}

///
/// setup di base
void setupMCU(void){

	// Set the system clock to run at 80Mhz off PLL with external crystal as
	// reference.
	//
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	/// abilita la porta F
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, BLUE_LED | GPIO_PIN_1 | GPIO_PIN_3);
	/// spegne i led
	GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED | GPIO_PIN_1, 0);
	//
	// Set the pin direction and mode.
	//
	/// PF4 in ingresso
	HWREG(GPIO_PORTF_BASE + GPIO_O_DIR) 	&= ~GPIO_PIN_4;
	/// no alternate function
	HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) 	&= ~GPIO_PIN_4;
	/// 2 ma di corrente
	HWREG(GPIO_PORTF_BASE + GPIO_O_DR2R)	|= GPIO_PIN_4;
	/// controllo slew rate off
	HWREG(GPIO_PORTF_BASE + GPIO_O_SLR)		&= ~GPIO_PIN_4;
	/// pull up
	HWREG(GPIO_PORTF_BASE + GPIO_O_PUR)		|= GPIO_PIN_4;
	/// abilitazione della funzione digitale
	HWREG(GPIO_PORTF_BASE + GPIO_O_DEN)		|= GPIO_PIN_4;
	/// legge il pin (ricordare lo shift di 2 posizione verso sinistra della maschera di bit
	///  p. 654 U.G.)
	while(HWREG(GPIO_PORTF_BASE + (GPIO_O_DATA + (GPIO_PIN_4 << 2))) == GPIO_PIN_4);

	//setup PB0 per scopi di debug
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	/// PB0 in uscita
	HWREG(GPIO_PORTB_BASE + GPIO_O_DIR) 	|= GPIO_PIN_0;
	/// no alternate function
	HWREG(GPIO_PORTB_BASE + GPIO_O_AFSEL) 	&= ~GPIO_PIN_0;
	/// 2 ma di corrente
	HWREG(GPIO_PORTB_BASE + GPIO_O_DR2R)	|= GPIO_PIN_0;
	/// controllo slew rate off
	HWREG(GPIO_PORTB_BASE + GPIO_O_SLR)		&= ~GPIO_PIN_0;
	/// pull up
	//HWREG(GPIO_PORTF_BASE + GPIO_O_PUR)		|= GPIO_PIN_4;
	/// abilitazione della funzione digitale
	HWREG(GPIO_PORTB_BASE + GPIO_O_DEN)		|= GPIO_PIN_0;
	/// imposta il pin (ricordare lo shift di 2 posizione verso sinistra della maschera di bit
	HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_0 << 2))) |=  GPIO_PIN_0;
	HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (GPIO_PIN_0 << 2))) &=  ~GPIO_PIN_0;


}

extern int procCom;
///
/// routine di attesa
void attesa(int delay){

	while (delay > 0)
		if (procCom == 1){
			procCom = 0;
			delay--;
		}

}

