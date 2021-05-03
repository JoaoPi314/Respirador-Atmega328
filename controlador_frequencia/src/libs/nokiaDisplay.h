/*---------------------------------------------------------------------	*
 * Arquivo: nokiaDisplay.h  											*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com protótipo da função que controla o que será   *
 * mostrado no display													*
 *---------------------------------------------------------------------	*/


#ifndef NOKIADISPLAY_H
#define NOKIADISPLAY_H

#include <avr/io.h>
#include "nokia5110.h"



void changeDisplayConfig(uint8_t sel, uint8_t freq, uint32_t card, uint16_t sat, float temp, const char* pressure, uint8_t o2Valv, uint8_t volume);

#endif