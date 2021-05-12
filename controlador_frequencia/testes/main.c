#include "ks0108.h"
#include <avr/pgmspace.h>


//teste da escrita com os caracteres ASCII

int main(void)
{
	DDRD = 0xFF; //pinos do PORTD como saída
	DDRC = 0xFF; //pinos do PORTC como saída
	//UCSR0B= 0x00; //para uso dos pinos do PORTD no Arduino

	ks0108_init();

	ks0108_write_char('J', 0, 0);

	while(1){

	} //código principal
}