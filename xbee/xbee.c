/*
 * xbee.c
 *
 *  Created on: 08/mar/2015
 *      Author: massimo
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "xbee.h"

extern volatile uint8_t uart1buffer[16], RX_PTR1, READ_PTR1;
extern int procCom;


///
/// funzioncina che prova se il modulo XBEE e' presente
uint32_t testXbee(){
	uint32_t i, stato = 0;
	READ_PTR1 = RX_PTR1 = 0;
	for (i = 0; i < 3; i++)
		while (ROM_UARTCharPutNonBlocking(UART1_BASE, '+') == false);
	i = 0;
	while (i < 100){
		if (procCom == 1){
			i++;
			procCom = 0;
		}
		if (READ_PTR1 != RX_PTR1){
			if (uart1buffer[READ_PTR1] == 'O' && stato == 0){
				/// e' arrivato il primo carattere della risposta
				stato = 1;
				READ_PTR1++;
			}
			if (uart1buffer[READ_PTR1] == 'K' && stato == 1){
				/// e' arrivato il secondo carattere della rispota
				stato = 2;
				READ_PTR1++;
				/// il modul e' presente
				return 0;
			}
			else{
				stato = 4;
				i = 100; /// esce forzatamente
				READ_PTR1++;
			}
		}
	}
	return 1;
}

