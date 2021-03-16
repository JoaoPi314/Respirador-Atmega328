/*---------------------------------------------------------------------	*
 * Arquivo: animateLed.h												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com o protótipo da função que anima a barra de	*
 * led, definição de constantes e inclusão das bibliotecas necessárias	*
 *---------------------------------------------------------------------	*/


#ifndef ANIMATELED_H_
#define ANIMATELED_H_

#ifndef F_CPU								//Prevenção caso tenha sido definido em outro arquivo ou em linha de comando
#define F_CPU 16000000UL					//Definição do clock da CPU
#endif

#include <avr/io.h>
#include "myDelay.h"


uint8_t animateLed(uint8_t FreqRespiracao);	//Protótipo da função

#endif /* ANIMATELED_H_ */