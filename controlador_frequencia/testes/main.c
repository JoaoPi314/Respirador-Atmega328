#include "ks0108.h"
#include <avr/pgmspace.h>


//teste da escrita com os caracteres ASCII

int main(void)
{
	DDRD = 0xFF; //pinos do PORTD como saída
	DDRC = 0xFF; //pinos do PORTC como saída
	DDRB = 0xFF;
	//UCSR0B= 0x00; //para uso dos pinos do PORTD no Arduino

	ks0108_init();

	ks0108_write_string("Dados vitais", 0, 0);
	ks0108_write_string("Params.", 85, 0);

	for(uint8_t i = 0; i < 127; i++){
		ks0108_write(0x01, i, 1);
	}
	for(uint8_t i = 0; i < 8; i++){
		ks0108_write(0xff, 73, i);
	}

	ks0108_write_string("12", 0, 2);
	ks0108_write_string("100", 0, 3);
	ks0108_write_string("36.5", 0, 4);
	ks0108_write_string("128x80", 0, 5);

	ks0108_write_string("bpm", 43, 2);
	ks0108_write_string("\%SpO2", 43, 3);
	ks0108_write_string("oC", 43, 4);
	ks0108_write_string("mmHg", 43, 5);

	ks0108_write_string("12", 76, 2);
	ks0108_write_string("100", 76, 3);
	ks0108_write_string("10", 76, 5);
	ks0108_write_string("ASS", 76, 6);

	ks0108_write_string("bpm", 99, 2);
	ks0108_write_string("\%SpO2", 99, 3);
	ks0108_write_string("oC", 99, 5);
	ks0108_write_string("mode", 99, 6);


	//serial_com(0x00, 0x00);
	set_bit(PORTC, 0);
	while(1){

	} //código principal
}

// #define F_CPU 16000000UL

// #include <avr/io.h>
// #include <util/delay.h>
// #include "mascaras.h"

// #define D PB0 //pino de dados para o 4094
// #define CLK PB1 //pino clock para o 4094
// #define STB PB2 //pino de strobe para o 4094
// #define pulso_CLK() set_bit(PORTB,CLK); _delay_ms(100); clr_bit(PORTB,CLK)
// #define pulso_STB() set_bit(PORTB,STB); _delay_ms(100); clr_bit(PORTB,STB)
// //----------------------------------------------------------------------------------
// // Sub-rotina que envia 1 byte para o 4094 - serial/paralelo
// //----------------------------------------------------------------------------------
// void serial_paral(uint16_t c)
// {
// unsigned char i=13; //envia primeiro o MSB
// do
// { i--;
// if(tst_bit(c,i)) //se o bit for 1, ativa o pino de DADOS
// set_bit(PORTB,D);
// else //se não, o zera
// clr_bit(PORTB,D);
// pulso_CLK();
// } while (i!=0);
// }
// //----------------------------------------------------------------------------------
// int main(void)
// {
// DDRB = 0b00000111; //pinos PB0:2 como saídas
// PORTB = 0b11111000; //zera saídas
// serial_paral(0b1101100110100);//envia os 3 dados para os 4094 (primeiro o 0x58)
// pulso_STB();/*depois de enviar os 3 dados dá o pulso de Strobe, neste instante os
// dados passam para as saída*/
// pulso_CLK();
// while(1)
// {} //laço infinito
// }
// //==================================================================================