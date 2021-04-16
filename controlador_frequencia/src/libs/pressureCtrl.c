/*---------------------------------------------------------------------	*
 * Arquivo: pressureCtrl.c												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com a definição da máquina de estado que verifica	*
 * a sequência da pressão arterial										*
 *---------------------------------------------------------------------	*/

#include "pressureCtrl.h"

/*
 * A função pressureMeasure() implementa a FSM (Finite state machine)
 * responsável pela identificação da sequência ;MMMxLLL:
 * Parâmetros: @message: String que será concatenada a cada chamada
 *			   @pressureChar: Byte atual da leitura da USART
 */

states pressureMeasure(char * message, uint8_t pressureChar){

	static states actualState = INIT;
	static states nextState = INIT;

	static uint8_t i;
	actualState = nextState;		//Após todo estado, o atual é atualizado

	//Processamento do próximo estado e concatenação da mensagem
	switch(actualState){
		case INIT:		//Estado idle, verifica o primeiro char
			nextState = (pressureChar == 0x3b) ? M1 : ERROR; 		//Só vai para o próximo estado caso o caractere seja ';'
			i = 0;
			break;

		case M1:		//Estado do primeiro dígito da máxima	
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? M2 : ERROR;	//Só vaí para M2 caso seja numérico
			message[i] = pressureChar;
			i++;
			break;

		case M2:		//Estado do segundo dígito da máxima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? M3 :		//Só vai para M3 caso seja numérico
						(pressureChar == 0x78) ? L1 : ERROR;							//Caso não, vai para L1 caso seja 'x'
			message[i] = pressureChar;
			i++;
			break;

		case M3:		//Estado do terceiro dígito da máxima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? X :		//Só vai para X caso seja numérico
						(pressureChar == 0x78) ? L1 : ERROR;							//Caso não, vai para L1 caso seja 'x'
			message[i] = pressureChar;
			i++;
			break;

		case X:			//Estado do caractere 'x' que divide a máxima e a mínima	
			nextState = (pressureChar == 0x78) ? L1 : ERROR;	//Só vai para L1 caso seja x
			message[i] = pressureChar;
			i++;
			break;

		case L1:		//Estado do primeiro dígito da mínima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? L2 : ERROR;		//Só vai para L2 caso seja numérico
			message[i] = pressureChar;
			i++;
			break;

		case L2:		//Estado do segundo dígito da mínima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? L3 :		//Só vai para L3 caso seja numérico
						(pressureChar == 0x3a) ? DONE : ERROR;						//Vai para DONE caso seja ':'
			message[i] = (nextState == L3) ? pressureChar : ' ';
			i++;
			break;

		case L3:		//Estado do terceiro dígito da mínima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? LAST : 	//Só vai para LAST caso seja numérico
						(pressureChar == 0x3a) ? DONE : ERROR;						//Só vai para DONE caso seja ':'
			message[i] = (nextState == LAST) ? pressureChar : ' ';
			i++;
			break;
		
		case LAST:
			nextState = (pressureChar == 0x3a) ? DONE : ERROR;						//Vai para DONE se ':'
			break;

		case DONE:		//Estado final
			nextState = (pressureChar == 0x3b) ? M1 : ERROR; 		//Só vai para M1 caso o caractere seja ';'
			i = 0;
			break;

		case ERROR:		//Estado padrão é ERROR
			nextState = (pressureChar == 0x3b) ? M1 : ERROR;		//Sai do erro com o ';'
			i = 0;
	}


	return nextState;
}