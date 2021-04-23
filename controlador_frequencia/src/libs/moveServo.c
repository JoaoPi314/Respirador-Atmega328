/*---------------------------------------------------------------------	*
 * Arquivo: moveServo.c												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descriçção: Arquivo com a definição da função moveServo()			*
 *---------------------------------------------------------------------	*/
#include "moveServo.h"

/*
 * Função: moveServo()
 * Parâmetros: Inteiro de 8 bits com a frequência de movimentação do servo
 * motor
 * Descrição: De acordo com a frequência passada para a função, o retorno da
 * função será o dutyCycle responsável pela posição do servo motor naquele
 * momento
 */

uint8_t moveServo(uint8_t FreqRespiracao){
	
	static uint8_t count = 0;				//Variável que irá definir quando a configuração de LEDs irá mudar
	static uint8_t pwm = 250;				//5% duty cycle (0.05 * 5000)
	if(count < 9){
		pwm += 28;
		count++;							//Aproximadamente + 22.5°
	}else{
		pwm -= 28;
		if(pwm == 250)
			count = 0;
	}

	return pwm;
	
}