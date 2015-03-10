/*
 * uart.h
 *
 *  Created on: 31/gen/2015
 *
 *  tiva c
 *  I2C bus
 *  Author: robocupjr 15
 */

#ifndef UART_H_
#define UART_H_

#define UART_CLK	16000000

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(uint32_t, int);
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);



#endif /* UART_H_ */
