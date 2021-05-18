#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include <stdlib.h>
#include "ks0108.h"
#include <string.h>

void screen1(uint32_t card, uint16_t o2, float temp, char* pressure, uint8_t freq, uint8_t o2Valv, uint8_t vol, uint8_t mode);
void show_vital(uint32_t card, uint16_t o2, float temp, char* pressure);
void show_params(uint8_t freq, uint8_t o2Valv, uint8_t vol, uint8_t mode);


#endif //DISPLAY_H