/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.h												*
 * Criador: Jo�o Pedro Melquiades Gomes      Mat: 118110077				*
 * Descri��o: Arquivo com o prot�tipo da fun��o que anima a barra de	*
 * led, defini��o de constantes e inclus�o das bibliotecas necess�rias	*
 *---------------------------------------------------------------------	*/


#ifndef ANIMATELED_H_
#define ANIMATELED_H_

#ifndef F_CPU								//Preven��o caso tenha sido definido em outro arquivo ou em linha de comando
#define F_CPU 16000000UL					//Defini��o do clock da CPU
#endif

#include <avr/io.h>
#include "myDelay.h"


uint8_t animateLed(uint8_t FreqRespiracao);	//Prot�tipo da fun��o

#endif /* ANIMATELED_H_ */