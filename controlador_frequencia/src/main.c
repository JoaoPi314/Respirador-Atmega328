/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com conjunto de instruções que irão para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/animateLed.h"
#include "libs/mascaras.h"


//Variável estática só válida dentro desse arquivo
static volatile uint8_t FreqRespiracao = 12;
	


ISR(INT0_vect){				//Interrupção externa 0 (PD2)
	if(FreqRespiracao < 30)	//Enquanto a frequência for menor que 30, incremente
		FreqRespiracao++;
}

ISR(INT1_vect){				//Interrupção externa 1 (PD3)
	if(FreqRespiracao > 5)	//Enquanto a frequência for maior que 5, decremente
		FreqRespiracao--;
}


int main(void)
{
	//GPIO
	//Definição de direção das portas
	DDRB |= 0b11111111;					//Todos os pinos da porta B serão de saída
	DDRD |= ~(0b00001100<<0);			//Aciona os pinos 2 e 3 da porta D como entrada
	DDRC |= 0b11111111;					//Todos os pinos da porta C serão de saída
	//Inicialização e pull-ups
	PORTD |= (0b00001100<<0);			//Aciona o pull-up interno para os pinos 0 e 1 da porta D
	PORTB |= 0b00000000;				//Inicialmente todas as saídas estão NLB em B

	//Interrupções
	EICRA = 0b00001010;					//Interrupções INT1 e INT0 ativadas na borda de descida
	EIMSK =	0b00000011;					//Ativa as interrupções INT1 e INT0
	sei();								//Bit SREG em 1 - Interrupções globais ativadas

	
    while (1) 
    {
			
		PORTC = FreqRespiracao;

		//Atribuiçãoo da configuração de LEDS à porta B
		PORTB = animateLed(FreqRespiracao);
		

		
    }
}
