/*---------------------------------------------------------------------	*
 * Arquivo: pressureCtrl.h												*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com o protótipo da máquina de estado que verifica	*
 * a sequência da pressão arterial										*
 *---------------------------------------------------------------------	*/

#ifndef PRESSURE_CTRL_H
#define PRESSURE_CTRL_H

#include <avr/io.h>

//Enum com os estados da sequência. Foi usado código gray
//para a função precisar trocar apenas 1 bit na maioria
//das transições de estado
typedef enum states{ERROR = 0b0000, //0		//Estado de destino para qualquer erro que houver
					INIT  = 0b0001, //1		//Estado inicial. Apenas quando chamada pela primeira vez
					M1    = 0b0011, //3		//Estado do primeiro dígito (apenas número)
					M2	  = 0b0010, //2		//Estado do segundo dígito (número ou x)
					M3    = 0b0110, //6		//Estado do terceiro dígito (número ou x)
					X     = 0b0111, //7		//Estado do quarto dígito (número ou X)
					L1    = 0b0101, //5		//Estado do quinto dígito (apenas número)
					L2    = 0b0100, //4		//Estado do sexto dígito (número ou :)
					L3    = 0b1100,	//C 	//Estado do sétimo dígito (número ou :)
					LAST  = 0b1101, //D 	//Estado do oitavo dígito (apenas :)
					DONE  = 0b1001	//9		//Estado final (; volta para M1)
				} states;


states pressureMeasure(char *message, uint8_t pressureMeasure);




#endif