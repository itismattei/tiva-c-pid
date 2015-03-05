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

//#############
//valore dei tokens

#define		ERRORE				255
#define		AVANTI				1
#define		INDIETRO			2
#define		STOP				0
#define		DESTRA				3		//(+90°)
#define		SINISTRA			4		//(-90°)
#define		GIRA_INDIETRO		5		//(180°)
#define		GIRA_GRADI			6


///
/// struttura che analizza la frase e ne mantiene il token del comando
typedef struct _syn_stat{
	int ST;
	uint8_t cmd[2];
	uint8_t l_cmd;
	uint8_t token;
} syn_stat;

void parse(syn_stat *);
void convertToToken(syn_stat *);

#endif /* INIT_H_ */
