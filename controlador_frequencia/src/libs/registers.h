/*---------------------------------------------------------------------	*
 * Arquivo: registers.h													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com protótipo de funções que inicializam os regis-*
 * tradores do atmega328p												*
 *---------------------------------------------------------------------	*/

#ifndef REGISTERS_H
#define REGISTERS_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


#include <avr/io.h>
#include <avr/interrupt.h>

void gpioSetup();
void timerSetup();
void interruptSetup();
void adcSetup();
void usartSetup();

#endif //REGISTERS_H