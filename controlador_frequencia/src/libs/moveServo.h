/*---------------------------------------------------------------------	*
 * Arquivo: moveServo.h													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com o protótipo da função que move o servo motor,	*
 * definição de constantes e inclusão das bibliotecas necessárias		*
 *---------------------------------------------------------------------	*/


#ifndef MOVESERVO_H
#define MOVESERVO_H

#ifndef F_CPU								//Prevenção caso tenha sido definido em outro arquivo ou em linha de comando
#define F_CPU 16000000UL					//Definição do clock da CPU
#endif

#include <avr/pgmspace.h>
#include <avr/io.h>



uint16_t moveServo(uint8_t volume);	//Prot�tipo da função

#endif /* MOVESERVO_H */