/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com conjunto de instruções que irão para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/animateLed.h"
#include "libs/nokiaDisplay.h"
#include "libs/mascaras.h"

//Variável estática só válida dentro desse arquivo
static volatile uint8_t FreqRespiracao = 12;
static volatile uint8_t displayConfigFlag = 1;

ISR(PCINT2_vect){				//Interrupção externa na porta D
	if(!tst_bit(PIND, 0))		//Caso PD0 tenha sido a causa da interrupção, aumente
		FreqRespiracao++;
	else if(!tst_bit(PIND, 1))	//Caso PD1 tenha sido a causa da interrupção, decremente
		FreqRespiracao--;
	else if(!tst_bit(PIND, 7))	//Caso PD7 tenha sido a causa da interrupção, mude o display
		cpl_bit(displayConfigFlag, 0);
	//Verificação de ultrapassagem de limites
	if(FreqRespiracao < 5)		
		FreqRespiracao = 5;
	else if(FreqRespiracao > 30)
		FreqRespiracao = 30;
}


int main(void)
{

	//LCD inits
	nokia_lcd_init();					//Inicia o display LCD
	nokia_lcd_clear();					//Limpa a tela inicialmente

	//GPIO
	//Definição de direção das portas
	DDRB |= 0b11111111;					//Todos os pinos da porta B serão de saída
	DDRD |= ~(0b10000011<<0);			//Aciona os pinos 0, 1 e 7 da porta D como entrada
	//Inicialização e pull-ups
	PORTD |= (0b10000011<<0);			//Aciona o pull-up interno para os pinos 0, 1 e 7 da porta D
	PORTB |= 0b00000000;				//Inicialmente todas as saídas estão NLB em B

	//Interrupções
	PCICR  = 0b00000100;					//Interrupções por mudança na porta D ativadas
	PCMSK2 = 0b10000011;					//Ativa as interrupções individuais dos pinos PD0, PD1 e PD7
	sei();								//Bit SREG em 1 - Interrupções globais ativadas

	

    while (1) 
    {
  		
  		changeDisplayConfig(displayConfigFlag, FreqRespiracao);//Plota o gráfico da frequência x tempo e indica a frequência atual
		
		//Atribuiçãoo da configuração de LEDS à porta B
		PORTB = animateLed(FreqRespiracao);	
		
    }
}
