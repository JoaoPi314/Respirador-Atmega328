/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.h												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com o protótipo da função que anima a barra de	*
 * led, definição de constantes e inclusão das bibliotecas necessárias	*
 *---------------------------------------------------------------------	*/


#ifndef ANIMATELED_H_
#define ANIMATELED_H_

#define F_CPU 16000000UL					//Definição do clock da CPU
#define __DELAY_BACKWARD_COMPATIBLE__		//Habilita que variáveis sejam passadas para _delay_ms()


#include <avr/io.h>
#include <util/delay.h>


uint8_t animateLed(uint8_t FreqRespiracao);	//Protótipo da função

#endif /* ANIMATELED_H_ */