/*
 * tiva_i2c.h
 *
 *  Created on: 31/gen/2015
 *
 *  tiva c
 *  I2C bus
 *  Author: robocupjr 15
 */

#ifndef TIVA_I2C_H_
#define TIVA_I2C_H_


void InitI2C0(void);
void  I2CReceiveN(uint8_t slave_addr, uint8_t reg, uint8_t numElem, uint8_t buff[]);
uint32_t I2CReceive(uint32_t slave_addr, uint8_t reg);
void I2CSend(uint8_t slave_addr, uint8_t num_of_args, ...);
void I2CSend_String(uint32_t slave_addr, char array[]);




#define BLUE_LED  		GPIO_PIN_2




#endif /* TIVA_I2C_H_ */
