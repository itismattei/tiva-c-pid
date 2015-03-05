/*
 * init.h
 *
 *  Created on: 05/mar/2015
 *      Author: robtics
 */

#ifndef INIT_H_
#define INIT_H_


#define 	DIM_READ_BUFF		16
#define		CHECK_SUM			0xA9

///
/// struttura che analizza la frase e ne mantiene il token del comando
typedef struct _syn_stat{
	int ST;
	uint8_t cmd[2];
	uint8_t l_cmd;
} syn_stat;

void parse(syn_stat *);
void convertToToken(syn_stat *);

#endif /* INIT_H_ */
