/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com conjunto de instruções que irão para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/animateLed.h"
#include "libs/nokiaDisplay.h"
#include "libs/mascaras.h"

//Variável estática só válida dentro desse arquivo
static volatile uint8_t FreqRespiracao = 12;
static volatile uint8_t displayConfigFlag = 0;
static volatile uint16_t tempo_ms = 0;
static volatile uint32_t FreqCardiaca = 60;
static volatile uint32_t last_period = 0;
static volatile uint16_t t_last = 0;
static volatile uint8_t ledFlag = 0;


//Interrupções
ISR(TIMER0_COMPA_vect);
ISR(TIMER1_CAPT_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(PCINT2_vect);


//funções
void init_registers();
void init_lcd();

//------------------------------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	init_registers();
	init_lcd();
	uint8_t result = 0;

    while (1){ 
  		changeDisplayConfig(displayConfigFlag, FreqRespiracao, FreqCardiaca);//Plota o gráfico da frequência x tempo e indica a frequência atual	
    	
    	if(ledFlag){
	    	result = animateLed(FreqRespiracao);				//Chama rotina de animação de leds

			if(tst_bit(result, 0))
				set_bit(PORTD, 6);
			else
				clr_bit(PORTD, 6);

			clr_bit(result, 0);
			PORTB = result;
			ledFlag = 0;
		}
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------


void init_registers(){
	//GPIO
	//Definição de direção das portas
	set_bit(DDRC, 6);
	
	DDRB |= 0b11111110;					//Todos os pinos da porta B serão de saída
	DDRD |= ~(0b10001100<<0);			//Aciona os pinos 2, 3 e 7 da porta D como entrada
	//Inicialização e pull-ups
	PORTD |= (0b10001100<<0);			//Aciona o pull-up interno para os pinos 2, 3 e 7 da porta D
	PORTB |= 0b00000001;				//Inicialmente todas as saídas estão NLB em B

	//Interrupções
	EICRA = 0b00001010;					//Interrupções INT1 e INT0 ativadas na borda de descida
	EIMSK =	0b00000011;					//Ativa as interrupções INT1 e INT0
	PCICR  = 0b00000100;				//Interrupções por mudança na porta D ativadas
	PCMSK2 = 0b10000000;				//Ativa a interrupção individual do pino PD7
	

	TCCR0A = 0b00000010;				//TC0 operando em modo CTC
	TCCR0B = 0b00000011;				//Liga TC0 com prescaler = 64
	OCR0A  = 249;						//TC0 conta até 249
	TIMSK0 = 0b00000010;				//Habilita interrupção por comparação com OCR0A

	TCCR1B = 0b01000101;				//Prescaler em 1024, captura no posedge

	set_bit(TIMSK1, ICIE1);				//Habilita interrupção por captura
	
	sei();								//Bit SREG em 1 - Interrupções globais ativadas
}

void init_lcd(){
	//LCD inits
	nokia_lcd_init();					//Inicia o display LCD
	nokia_lcd_clear();					//Limpa a tela inicialmente
}




ISR(TIMER1_CAPT_vect){

	cpl_bit(TCCR1B, 6);
	if(ICR1 > t_last && t_last){
	
		FreqCardiaca = 2 * last_period * 64;
		FreqCardiaca /= 1000;
		FreqCardiaca = 60000/FreqCardiaca;
	
		last_period = (ICR1 - t_last);
	}

	t_last = ICR1;

}

ISR(TIMER0_COMPA_vect){			//Interrupção por overflow do TC0
	tempo_ms++;					//Conta o tempo após 1ms
	if((tempo_ms % (60000/(FreqRespiracao*16))) == 0){		//Caso o tempo atinja 1/16 do período
		ledFlag = 1;
	}
}

ISR(INT0_vect){					//Interrupção externa em PD2
	if(FreqRespiracao < 30)
		FreqRespiracao++;
}

ISR(INT1_vect){					//Interrupção externa em PD3
	if(FreqRespiracao > 5)
		FreqRespiracao--;
}

ISR(PCINT2_vect){				//Interrupção externa na porta D

	if(!tst_bit(PIND, 7))	//Caso PD7 tenha sido a causa da interrupção, mude o display
		cpl_bit(displayConfigFlag, 0);
	
}
