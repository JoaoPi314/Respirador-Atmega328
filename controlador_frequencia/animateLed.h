/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.h												*
 * Criador: Jo�o Pedro Melquiades Gomes      Mat: 118110077				*
 * Descri��o: Arquivo com o prot�tipo da fun��o que anima a barra de	*
 * led, defini��o de constantes e inclus�o das bibliotecas necess�rias	*
 *---------------------------------------------------------------------	*/


#ifndef ANIMATELED_H_
#define ANIMATELED_H_

#define F_CPU 16000000UL					//Defini��o do clock da CPU
#define __DELAY_BACKWARD_COMPATIBLE__		//Habilita que vari�veis sejam passadas para _delay_ms()


#include <avr/io.h>
#include <util/delay.h>


uint8_t animateLed(uint8_t FreqRespiracao);	//Prot�tipo da fun��o

#endif /* ANIMATELED_H_ */