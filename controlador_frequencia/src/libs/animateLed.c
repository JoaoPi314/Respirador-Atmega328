/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.c												*
 * Criador: Jo�o Pedro Melquiades Gomes      Mat: 118110077				*
 * Descri��o: Arquivo com a defini��o da fun��o animateLed()			*
 *---------------------------------------------------------------------	*/

#include "animateLed.h"

/*
 * Fun��o: animateLed()
 * Par�metros: Inteiro de 8 bits com a frequ�ncia de oscila��o da barra de LED
 * Descri��o: De acordo com a frequ�ncia passada para a fun��o, o retorno da
 * fun��o ser� a configura��o de LED correta para aquele momento no tempo,
 * e um delay � aplicado antes de sair da fun��o.
*/

uint8_t animateLed(uint8_t FreqRespiracao){
	static uint8_t countLeds = 0;			//Contador static que controlar� o acendimento dos leds
	static uint8_t result  = 0b00000000;	//Vari�vel em que a combina��o de portas ser� armazenada
	
	if(countLeds < 8){						//Caso o contador n�o tenha chegado a 8
		result = (result<<1) + 0b1;			//A cada chamada da fun��o acende mais um LED
		countLeds ++;
	}else{									//Caso o contador chegue a 8
		result >>= 1;						//A cada chamada, apaga um LED
		if(!result)							//Caso todos estejam apagados
		countLeds = 0;						//Reinicia o contador
	}
			
	myDelay(60000/(FreqRespiracao*16));	//Espera 1/16 do per�odo de oscila��o atual
	return result;
	
}