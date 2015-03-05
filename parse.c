/*
 * parse.c
 *
 *  Created on: 05/mar/2015
 *      Author: robotics
 */

#include <stdint.h>
#include "init.h"

extern volatile uint8_t uart1buffer[16], RX_PTR1, READ_PTR1;



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
	break;

	case 1:
		if (STATO->l_cmd == 2){
			/// si analizza il checksum e poi si esegue il comando
			if (STATO->cmd[0] ^ CHECK_SUM == STATO->cmd[1]){
				// ok
				convertToToken(STATO);
			}
		}

	break;

	}

}



void convertToToken(syn_stat *STATO){
}



