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
static volatile uint32_t FreqCardiaca = 0;
static volatile uint32_t last_period = 0;
static volatile uint16_t t_last = 0;


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

    while (1){ 
  		changeDisplayConfig(displayConfigFlag, FreqRespiracao, FreqCardiaca);//Plota o gráfico da frequência x tempo e indica a frequência atual	
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------


/*
 * Função que inicializa os registradores que serão utilizados
 */

void init_registers(){
	//GPIO
	//Definição de direção das portas
	set_bit(DDRC, 6);
	
	DDRB |= 0b11111110;					//Todos os pinos da porta B serão de saída, exceto PB0 (ICP1)
	DDRD |= ~(0b10001100<<0);			//Aciona os pinos 2, 3 e 7 da porta D como entrada (resto saída)
	//Inicialização e pull-ups
	PORTD |= (0b10001100<<0);			//Aciona o pull-up interno para os pinos 2, 3 e 7 da porta D
	PORTB |= 0b00000001;				//Inicialmente todas as saídas estão NLB em B e PB0 com pull-up

	//Interrupções
	EICRA = 0b00001010;					//Interrupções INT1 e INT0 ativadas na borda de descida
	EIMSK =	0b00000011;					//Ativa as interrupções INT1 e INT0
	PCICR  = 0b00000100;				//Interrupções por mudança na porta D ativadas
	PCMSK2 = 0b10000000;				//Ativa a interrupção individual do pino PD7
	

	TCCR0A = 0b00000010;				//TC0 operando em modo CTC
	TCCR0B = 0b00000011;				//Liga TC0 com prescaler = 64
	OCR0A  = 249;						//TC0 conta até 249
	TIMSK0 = 0b00000010;				//Habilita interrupção por comparação com OCR0A

	TCCR1B = 0b01000101;				//Prescaler em 1024, captura no posedge (TC1)

	TCCR2A = 0b00000010;				//TC2 operando em modo CTC
	TCCR0B = 0b00000011;				//Prescaler = 64


	set_bit(TIMSK1, ICIE1);				//Habilita interrupção por captura de TC1
	
	sei();								//Bit SREG em 1 - Interrupções globais ativadas
}

/*
 * função que inicializa o display LCD
 */


void init_lcd(){
	//LCD inits
	nokia_lcd_init();					//Inicia o display LCD
	nokia_lcd_clear();					//Limpa a tela inicialmente
}

/*
 * Interrupção em TC1 por captura de ICP1
 */


ISR(TIMER1_CAPT_vect){

	cpl_bit(TCCR1B, 6);								//Após uma interrupção no posedge, a próxima será no negedge
	if(ICR1 > t_last && t_last){					//Se o valor atual de ICR1 for maior que o valor anterior (sem OVF)
		if(last_period != 0){						//Se o período calculado anteriormente não for 0
			FreqCardiaca = 2 * last_period * 64;	//Início do cálculo da frequência -> Multiplica por 2 pois ICR1 - t_last é o tempo de borda alta/baixa
			FreqCardiaca /= 1000;					//Nesse ponto, a frequência está armazenando o período em milisegundos
			FreqCardiaca = 60000/FreqCardiaca;		//Converte para bpm
		}
		last_period = (ICR1 - t_last);				//Atualiza o valor anterior do período com o valor atual
	}

	t_last = ICR1;									//Atualiza o valor anterior do ICR1 com o valor atual

}

/*
 * Interrupção em TC0 por comparação com OCR0A
 */ 

ISR(TIMER0_COMPA_vect){										//Interrupção por overflow do TC0
	tempo_ms++;												//Conta o tempo após 1ms
	uint8_t result = 0;										//Variável que armazenará temporariamente a configuração dos LEDS
		
	//Essa lógica foi utilizada pois o pino PB0 é o ICP1, utilizado para captura no TC1

	if((tempo_ms % (60000/(FreqRespiracao*16))) == 0){		//Caso o tempo atinja 1/16 do período
		result = animateLed(FreqRespiracao);				//Chama rotina de animação de leds

		//Lógica para atribuir o LED0 (antes em PB0) para PD6
		if(tst_bit(result, 0))								
			set_bit(PORTD, 6);
		else
			clr_bit(PORTD, 6);

		//Atribui individualmente os LEDs à PORTB para não interferir com o valor de PB0

		for(uint8_t i = 7; i > 0; i--){
			if(tst_bit(result, i))								
				set_bit(PORTB, i);
			else
				clr_bit(PORTB, i);		
		}
	}
}

/*
 *Interrupção externa em PD2
 */

ISR(INT0_vect){					
	if(FreqRespiracao < 30)			//Aumenta a frequência caso esteja dentro do limite (negedge do PD2)
		FreqRespiracao++;
}

/*
 *Interrupção externa em PD2
 */

ISR(INT1_vect){				
	if(FreqRespiracao > 5)			//Diminui a frequência caso esteja dentro do limite (negedge do PD3)
		FreqRespiracao--;
}
/*
 * Interrupção externa na porta D
 */

ISR(PCINT2_vect){				

	if(!tst_bit(PIND, 7))	//Caso PD7 tenha sido a causa da interrupção, mude o display
		cpl_bit(displayConfigFlag, 0);
	
}
