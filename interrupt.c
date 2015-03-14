/*
 * interrupt.c
 *
 *  Created on: 06/mar/2015
 *      Author: robotics
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "uartp/uartstdio.h"
#include "uartp/uart.h"
#include "init.h"


///
/// routine di servizio della uart1
volatile uint8_t uart1buffer[16], RX_PTR1 = 0, READ_PTR1 = 0;

void UART1IntHandler(void)
{
    uint32_t 	ui32Status;
    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART1_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART1_BASE)){
        //
        // Read the next character from the UART and write it back to the UART.
        //
    	uart1buffer[RX_PTR1]=ROM_UARTCharGetNonBlocking(UART1_BASE);
    	/// echo
    	ROM_UARTCharPutNonBlocking(UART1_BASE, uart1buffer[RX_PTR1]);
    	RX_PTR1++;
    	RX_PTR1 &= DIM_READ_BUFF;
        //UARTCharPutNonBlocking(UART1_BASE,
        //		dato);
    }
}
