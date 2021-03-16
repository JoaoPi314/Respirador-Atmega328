/*---------------------------------------------------------------------	*
 * Arquivo: myDelay.h 													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com o protótipo da função que implementa delay va-*
 * riável																*
 *---------------------------------------------------------------------	*/

#ifndef MYDELAY__H
#define MYDELAY_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


void myDelay(uint16_t ms);

#endif