#ifndef KS0108_H
#define KS0108_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "mascaras.h"


#define POWER_LCD 0x3f		//Comando para ligar o LCD
#define OFF_LCD   0x3e		//Comando para desligar o LCD
#define ROW_INIT  0xc0		//Comando para iniciar linha
#define Y_INIT    0xb8		//Comando para iniciar Y
#define PAG_INIT  0xb8		//Comando para iniciar página

//Temporario
#define DATA PORTD
#define CTRL PORTC
/////////
#define RST PC0
#define CS1 PC1
#define CS2 PC2
#define EN  PC4
#define RW  PC5
#define RS  PC6

#define set_write() 	_delay_us(1); clr_bit(CTRL, RW)		//Configura modo de escrita
#define set_read()  	_delay_us(1); set_bit(CTRL, RW)		//Configura modo de leitura 
#define set_data()  	_delay_us(1); set_bit(CTRL, RS)		//Configura envio de dados
#define set_inst()		_delay_us(1); clr_bit(CTRL, RS)		//Configura envio de instrução

#define set_CS1()   	_delay_us(1); set_bit(CTRL, CS1)	//Configura primeira metade do LCD
#define set_CS2()   	_delay_us(1); set_bit(CTRL, CS2)	//Configura segunda metade do LCD
#define clr_CS1()   	_delay_us(1); clr_bit(CTRL, CS1)	//Inativa primeira metade do LCD
#define clr_CS2()   	_delay_us(1); clr_bit(CTRL, CS2)	//Inativa segunda metade do LCD
#define set_enable()	_delay_us(1); set_bit(CTRL, EN)		//Ativa o enable
#define clr_enable()	_delay_us(1); clr_bit(CTRL, EN)		//Desativa o enable
#define set_rst()		_delay_us(1); set_bit(CTRL, RST)	//Ativa reset
#define clr_rst()		_delay_us(1); clr_bit(CTRL, RST)	//Desativa reset

#define reset_LCD()		clr_rst(); set_rst()				//Pulso de reset
#define enable_LCD()	set_enable(), clr_enable()			//Pulso de enable


/////////////////////////////////////////FUNÇÕES///////////////////////////////////////////

void ks0108_init();
void ks0108_write(uint8_t data, uint8_t col, uint8_t page);
void ks0108_clear();
void ks0108_write_char(uint8_t c, uint8_t col, uint8_t page);
void ks0108_write_string(uint8_t type, uint8_t *str, uint8_t col, uint8_t page);


#endif //KS0108_H