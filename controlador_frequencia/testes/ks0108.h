#ifndef KS0108_H
#define KS0108_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "mascaras.h"


#define POWER_LCD 0x3f		//Comando para ligar o LCD
#define OFF_LCD   0x3e		//Comando para desligar o LCD
#define ROW_INIT  0xc0		//Comando para iniciar linha
#define Y_INIT    0x40		//Comando para iniciar Y
#define PAG_INIT  0xb8		//Comando para iniciar página


//Byte de configuração:
/*

|    0     |    0     |     0     |     0    |    CS2   |    CS1    |    RW    |    RS    |

*/


/////
#define RS  0
#define RW  1
#define CS1 2
#define CS2 3

#define CTRL_PORT PORTB
#define RST PB0
#define STB PB1
#define CLK PB2
#define DT  PB3
#define EN  PB4

#define STB_pulse() set_bit(CTRL_PORT, STB); _delay_us(1); clr_bit(CTRL_PORT, STB)
#define CLK_pulse() set_bit(CTRL_PORT, CLK); _delay_us(1); clr_bit(CTRL_PORT, CLK)


#define set_write(CTRL) 	_delay_us(1); clr_bit(CTRL, RW)		//Configura modo de escrita
#define set_read(CTRL)  	_delay_us(1); set_bit(CTRL, RW)		//Configura modo de leitura 
#define set_data(CTRL)  	_delay_us(1); set_bit(CTRL, RS)		//Configura envio de dados
#define set_inst(CTRL)		_delay_us(1); clr_bit(CTRL, RS)		//Configura envio de instrução

#define set_CS1(CTRL)   	_delay_us(1); set_bit(CTRL, CS1)	//Configura primeira metade do LCD
#define set_CS2(CTRL)   	_delay_us(1); set_bit(CTRL, CS2)	//Configura segunda metade do LCD
#define clr_CS1(CTRL)   	_delay_us(1); clr_bit(CTRL, CS1)	//Inativa primeira metade do LCD
#define clr_CS2(CTRL)   	_delay_us(1); clr_bit(CTRL, CS2)	//Inativa segunda metade do LCD
#define set_enable()		_delay_us(1); set_bit(CTRL_PORT, EN)		//Ativa o enable
#define clr_enable()		_delay_us(1); clr_bit(CTRL_PORT, EN)		//Desativa o enable
#define set_rst(CTRL)		_delay_us(1); set_bit(CTRL_PORT, RST)	//Ativa reset
#define clr_rst(CTRL)		_delay_us(1); clr_bit(CTRL_PORT, RST)	//Desativa reset

#define reset_LCD()			clr_rst(); set_rst()				//Pulso de reset
#define enable_LCD()		set_enable(), clr_enable()			//Pulso de enable




/////////////////////////////////////////FUNÇÕES///////////////////////////////////////////

void ks0108_init();
void ks0108_write(uint8_t data, uint8_t col, uint8_t page);
void ks0108_clear();
void ks0108_write_char(uint8_t c, uint8_t col, uint8_t page);
void ks0108_write_string(char *str, uint8_t col, uint8_t page);


//////////////////////////// SERIAL /////////////////////////////////////

void serial_com(uint8_t ctrl, uint8_t data);


#endif //KS0108_H