/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: Jo�o Pedro Melquiades Gomes      Mat: 118110077				*
 * Descri��o: Arquivo com conjunto de instru��es que ir�o para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <avr/io.h>
#include "animateLed.h"

int main(void)
{
	//Configura��o das portas
	DDRB |= 0b11111111;					//Todos os pinos da porta B ser�o de sa�da
	DDRD |= ~(0b00000011<<0);			//Aciona os pinos 0 e 1 da porta D como entrada
	DDRC |= 0b11111111;					//Todos os pinos da porta C serão de saída

	PORTD |= (0b00000011<<0);			//Aciona o pull-up interno para os pinos 0 e 1 da porta D
	PORTB |= 0b00000000;				//Inicialmente todas as sa�das est�o NLB em B
	//Vari�veis locais
	uint8_t FreqRespiracao = 12;		//Respira��o normal do ser humano em condi��es normais
		
	PORTC = FreqRespiracao;
	
    while (1) 
    {
		//Aumentar a frequ�ncia tem prioridade caso os dois bot�es sejam apertados ao mesmo tempo
		if(!(PIND & (1<<0))){			//Se o pino 0 da porta D for 0, aumentar a frequ�ncia
			FreqRespiracao ++;
		} else if(!(PIND & (1<<1))){	//Se o pino 1 da porta D for 0, diminuir a frequ�ncia
			FreqRespiracao --;
		}
		
		//Verifica se a frequ�ncia de respira��o ultrapassou os limites
		if(FreqRespiracao < 5)
			FreqRespiracao ++;
		else if(FreqRespiracao > 30)
			FreqRespiracao --;
		
		PORTC = FreqRespiracao;

		//Atribi��o da configura�� ode LEDS � porta B
		PORTB = animateLed(FreqRespiracao);
		

		
    }
}
