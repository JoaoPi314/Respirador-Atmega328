/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com conjunto de instruções que irão para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <avr/io.h>
#include "libs/animateLed.h"
#include "libs/mascaras.h"

int main(void)
{
	//Configuração das portas
	DDRB |= 0b11111111;					//Todos os pinos da porta B serão de saída
	DDRD |= ~(0b00000011<<0);			//Aciona os pinos 0 e 1 da porta D como entrada
	DDRC |= 0b11111111;					//Todos os pinos da porta C serão de saída

	PORTD |= (0b00000011<<0);			//Aciona o pull-up interno para os pinos 0 e 1 da porta D
	PORTB |= 0b00000000;				//Inicialmente todas as saídas estão NLB em B
	//Variáveis locais
	uint8_t FreqRespiracao = 12;		//Respiração normal do ser humano em condições normais
		
	PORTC = FreqRespiracao;
	
    while (1) 
    {
		//Aumentar a frequência tem prioridade caso os dois botôes sejam apertados ao mesmo tempo
		if(!(PIND & (1<<0))){			//Se o pino 0 da porta D for 0, aumentar a frequência
			FreqRespiracao ++;
		} else if(!(PIND & (1<<1))){	//Se o pino 1 da porta D for 0, diminuir a frequência
			FreqRespiracao --;
		}
		
		//Verifica se a frequência de respiração ultrapassou os limites
		if(FreqRespiracao < 5)
			FreqRespiracao ++;
		else if(FreqRespiracao > 30)
			FreqRespiracao --;
		
		PORTC = FreqRespiracao;

		//Atribuiçãoo da configuração de LEDS à porta B
		PORTB = animateLed(FreqRespiracao);
		

		
    }
}
