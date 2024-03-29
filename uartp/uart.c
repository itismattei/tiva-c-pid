/*
 * uart.c
 *
 *  Created on: 31/gen/2015
 *
 *  tiva c
 *  I2C bus
 *  Author: robocupjr 15
 */


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "uartstdio.h"
#include "uart.h"


//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(uint32_t baud, int numUart){

	volatile uint32_t tempo;
	if (numUart == 0){
		//
		// Enable the GPIO Peripheral used by the UART.
		//
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

		//
		// Enable UART0
		//
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

		//
		// Configure GPIO Pins for UART mode.
		//
		ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
		ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
		ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

		//
		// Use the internal 16MHz oscillator as the UART clock source.
		//
		UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

		//
		// Initialize the UART for console I/O.
		//
		UARTStdioConfig(numUart, baud, UART_CLK);
	}
	else{
		//
		// Enable the GPIO Peripheral used by the UART.
		//
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

		//
		// Enable UART1
		//
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

		//
		// Configure GPIO Pins for UART mode.
		//
		ROM_GPIOPinConfigure(GPIO_PB0_U1RX);
		ROM_GPIOPinConfigure(GPIO_PB1_U1TX);
		ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

		//
		// Use the internal 16MHz oscillator as the UART clock source.
		//
		UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);

		//
		// Initialize the UART for console I/O.
		//
		UARTStdioConfig(1, baud, UART_CLK);
	}
}


//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}


///
/// routine di servizio della uart1
volatile uint8_t uart1buffer[16], RX_PTR1 = 0, TX_PTR1 = 0;

void UART1IntHandler(void)
{
    uint32_t 	ui32Status, dato;
    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART1_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART1_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART1_BASE)){
        //
        // Read the next character from the UART and write it back to the UART.
        //
    	uart1buffer[RX_PTR1++]=UARTCharGetNonBlocking(UART1_BASE);
    	RX_PTR1 &= 0xF;
        //UARTCharPutNonBlocking(UART1_BASE,
        //		dato);
    }
}


