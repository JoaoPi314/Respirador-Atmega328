/*---------------------------------------------------------------------	*
 * Arquivo: registers.h													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com protótipo de funções que inicializam os regis-*
 * tradores do atmega328p												*
 *---------------------------------------------------------------------	*/

#ifndef REGISTERS_H
#define REGISTERS_H

#include <avr/io.h>
#include <avr/interrupt.h>

void gpioSetup();
void timerSetup();
void interruptSetup();
void adcSetup();

#endif //REGISTERS_H