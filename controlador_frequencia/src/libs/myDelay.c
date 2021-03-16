/*---------------------------------------------------------------------	*
 * Arquivo: myDelay.c 													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com a declaração da função que implementa delay 	*
 * variável																*
 *---------------------------------------------------------------------	*/

#include "myDelay.h"

void myDelay(uint16_t ms){
    
    for(uint16_t i = ms; i > 0; i--){
    _delay_ms(1);
    }

}