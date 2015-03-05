/*
 * parse.c
 *
 *  Created on: 05/mar/2015
 *      Author: robotics
 */

#include <stdint.h>
#include "init.h"

extern volatile uint8_t uart1buffer[16], RX_PTR1, READ_PTR1;

void resetAutoma(syn_stat * STATO){
	STATO->ST = 0;
	STATO->cmd[0] = STATO->cmd[1] = 0;
	STATO->l_cmd = 0;
}

///
/// analizza il comando che e' arrivato
///
void parse(syn_stat *STATO){


	STATO->cmd[STATO->ST] = uart1buffer[READ_PTR1];

	switch(STATO->ST){
	case 0:
		if (STATO->cmd[0] >64 && STATO->cmd[0] < 91 ){
			/// una lettera MAIUSCOLA e quindi un comand di azione da raspberry
			STATO->l_cmd = 2;
			STATO->ST = 1;
		}
		else if(STATO->cmd[0] <= 16){
			/// comando di richiesta dati lungo due caratteri
			STATO->l_cmd = 2;
			STATO->ST = 1;
		}
	break;

	case 1:
		if (STATO->l_cmd == 2){
			/// si analizza il checksum e poi si esegue il comando
			if (STATO->cmd[0] ^ CHECK_SUM == STATO->cmd[1]){
				// ok
				convertToToken(STATO);
				// resetta l'automa
				resetAutoma(STATO);
			}
		}

	break;

	}
}


///
/// converte l'indicatoredi uncomand in un token
void convertToToken(syn_stat *STATO){

	switch(STATO->cmd[0]){
	case 'A':
		STATO->token = AVANTI;
	break;
	case 'I':
		STATO->token = INDIETRO;
	break;
	case 'S':
		STATO->token = STOP;
	break;
	case 'R':
		STATO->token = DESTRA;
	break;
	case 'L':
		STATO->token = SINISTRA;
	break;
	case 'B':
		STATO->token = GIRA_INDIETRO;
	break;
	case 'G':
		//// non ancora implementato
		STATO->token = ERRORE;
	break;
	default:
		/// se nessun comando e'giusto produce un errore.
		STATO->token = ERRORE;
	break;
	}

	/// potrebbe essere un comando nnuerico di richiesta dati
	if (STATO->cmd[0] <= 16){

	}

}



