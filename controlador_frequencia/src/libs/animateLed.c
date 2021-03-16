/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.c												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com a definição da função animateLed()			*
 *---------------------------------------------------------------------	*/

#include "animateLed.h"

/*
 * Função: animateLed()
 * Parâmetros: Inteiro de 8 bits com a frequência de oscilação da barra de LED
 * Descrição: De acordo com a frequência passada para a função, o retorno da
 * função será a configuração de LED correta para aquele momento no tempo,
 * e um delay é aplicado antes de sair da função.
*/

uint8_t animateLed(uint8_t FreqRespiracao){
	static uint8_t countLeds = 0;			//Contador static que controlará o acendimento dos leds
	static uint8_t result  = 0b00000000;	//Variável em que a combinação de portas será armazenada
	
	if(countLeds < 8){						//Caso o contador não tenha chegado a 8
		result = (result<<1) + 0b1;			//A cada chamada da função acende mais um LED
		countLeds ++;
	}else{									//Caso o contador chegue a 8
		result >>= 1;						//A cada chamada, apaga um LED
		if(!result)							//Caso todos estejam apagados
		countLeds = 0;						//Reinicia o contador
	}
			
	myDelay(60000/(FreqRespiracao*16));	//Espera 1/16 do período de oscilação atual
	return result;
	
}