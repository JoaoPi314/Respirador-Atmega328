/*---------------------------------------------------------------------	*
 * Arquivo: oledDisplay.h 	  											*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com protótipo da função que controla o que será   *
 * mostrado no display													*
 *---------------------------------------------------------------------	*/


#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <avr/io.h>
#include "lcd.h"
//#include "lcd.c"
void initLED();
//void changeDisplayConfig(uint8_t sel, uint8_t freq, uint32_t card, uint16_t sat, float temp, const char* pressure, uint8_t o2Valv, uint8_t volume, uint8_t breathMode);

#endif