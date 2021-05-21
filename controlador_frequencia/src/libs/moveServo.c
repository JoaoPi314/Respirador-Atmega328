/*---------------------------------------------------------------------	*
 * Arquivo: moveServo.c													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com a definição da função moveServo()				*
 *---------------------------------------------------------------------	*/
#include "moveServo.h"

/*
 * Função: moveServo()
 * Parâmetros: Inteiro de 8 bits com o volume atual de respiração
 * Descrição: De acordo com a frequência passada para a função, o retorno da
 * função será o dutyCycle responsável pela posição do servo motor naquele
 * momento
 */

uint16_t moveServo(uint8_t volume){
	
	static uint8_t count = 0;				//Variável que irá definir quando a configuração de LEDs irá mudar
	static uint16_t pwm = 12;				//~5% duty cycle (0.05 * 256)



	if(count < volume){					//Movimenta de acordo com o volume de ar injetado
		pwm += 1;
		count++;							
	}else{
		pwm -= 1;
		if(pwm == 12)
			count = 0;
	}
	


	return pwm;
	
}