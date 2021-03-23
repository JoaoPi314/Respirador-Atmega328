/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.c												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descriçção: Arquivo com a definição da função animateLed()			*
 *---------------------------------------------------------------------	*/
#include "animateLed.h"

/*
 * Função: animateLed()
 * Parâmetros: Inteiro de 8 bits com a frequência de oscilação da barra de LED
 * Inteiro de 16 bits com o tempo atual com o atmega ligado
 * Descrição: De acordo com a frequência passada para a função, o retorno da
 * função será a configuração de LED correta para aquele momento no tempo,
 * e um delay é aplicado antes de sair da função.
*/

uint8_t animateLed(uint8_t FreqRespiracao){
	

	static uint8_t result  = 0b00000000;		//Variável em que a combinação de portas será armazenada
	static uint8_t countLeds = 0;				//Variável que irá definir quando a configuração de LEDs irá mudar


	if(countLeds < 8){						//Caso o contador não tenha chegado a 8
		result = (result<<1) + 0b1;			//A cada chamada da função acende mais um LED
		countLeds ++;
	}else{									//Caso o contador chegue a 8
		result >>= 1;						//A cada chamada, apaga um LED
		if(!result)							//Caso todos estejam apagados
		countLeds = 0;						//Reinicia o contador
	}
	
	return result;
	
}