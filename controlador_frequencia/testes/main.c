// #include "ks0108.h"
// #include <avr/pgmspace.h>


// //teste da escrita com os caracteres ASCII

// int main(void)
// {
// 	DDRD = 0xFF; //pinos do PORTD como saída
// 	DDRC = 0xFF; //pinos do PORTC como saída
// 	DDRB = 0xFF;
// 	//UCSR0B= 0x00; //para uso dos pinos do PORTD no Arduino

// 	//ks0108_init();

// 	//ks0108_write_char('J', 0, 0);

// 	serial_com(0x00, 0x00);

// 	set_bit(PORTC, 0);
// 	while(1){

// 	} //código principal
// }

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "mascaras.h"

#define D PB0 //pino de dados para o 4094
#define CLK PB1 //pino clock para o 4094
#define STB PB2 //pino de strobe para o 4094
#define pulso_CLK() set_bit(PORTB,CLK); _delay_ms(100); clr_bit(PORTB,CLK)
#define pulso_STB() set_bit(PORTB,STB); _delay_ms(100); clr_bit(PORTB,STB)
//----------------------------------------------------------------------------------
// Sub-rotina que envia 1 byte para o 4094 - serial/paralelo
//----------------------------------------------------------------------------------
void serial_paral(unsigned char c)
{
unsigned char i=8; //envia primeiro o MSB
do
{ i--;
if(tst_bit(c,i)) //se o bit for 1, ativa o pino de DADOS
set_bit(PORTB,D);
else //se não, o zera
clr_bit(PORTB,D);
pulso_CLK();
} while (i!=0);
}
//----------------------------------------------------------------------------------
int main(void)
{
unsigned char j;
unsigned char Dados[3]= {0x58, 0xF1, 0xAA};
DDRB = 0b00000111; //pinos PB0:2 como saídas
PORTB = 0b11111000; //zera saídas
for(j=0; j<3;j++)
serial_paral(Dados[j]);//envia os 3 dados para os 4094 (primeiro o 0x58)
pulso_STB();/*depois de enviar os 3 dados dá o pulso de Strobe, neste instante os
dados passam para as saída*/
pulso_CLK();
while(1)
{} //laço infinito
}
//==================================================================================