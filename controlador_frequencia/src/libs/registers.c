/*---------------------------------------------------------------------	*
 * Arquivo: registers.h													*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com definição de funções que inicializam os regis-*
 * tradores do atmega328p												*
 *---------------------------------------------------------------------	*/

#include "registers.h"
#include "mascaras.h"


void gpioSetup(){
	//GPIO
	//Definição de direção das portas
	DDRC |= 0b11111100;
	DDRB |= 0b11111110;					//Todos os pinos da porta B serão de saída
	DDRD |= ~(0b10001100<<0);			//Aciona os pinos 2, 3 e 7 da porta D como entrada
	
	//Inicialização e pull-ups
	PORTD |= (0b10001100<<0);			//Aciona o pull-up interno para os pinos 2, 3 e 7 da porta D
	PORTB |= 0b00000001;				//Inicialmente todas as saídas estão NLB em B
}



void timerSetup(){
	//Configuração dos timers
	//TC0
	TCCR0A = 0b00000010;				//TC0 operando em modo CTC
	TCCR0B = 0b00000011;				//Liga TC0 com prescaler = 64
	OCR0A  = 249;						//TC0 conta até 249

	//TC1
	TCCR1B = 0b01000101;				//Prescaler em 1024, captura no posedge
	TCNT1 = 0x00;
}


void interruptSetup(){
	//Interrupções externas
	EICRA = 0b00001010;					//Interrupções INT1 e INT0 ativadas na borda de descida
	EIMSK =	0b00000011;					//Ativa as interrupções INT1 e INT0
	PCICR  = 0b00000100;				//Interrupções por mudança na porta D ativadas
	PCMSK2 = 0b10000000;				//Ativa a interrupção individual do pino PD7

	//Timers
	TIMSK0 = 0b00000010;				//Habilita interrupção por comparação com OCR0A

	set_bit(TIMSK1, ICIE1);				//Habilita interrupção por captura no TC1

	sei();								//Bit SREG em 1 - Interrupções globais ativadas
}


void adcSetup(){
	//configuração ADC
	ADMUX = 0b01000001;					//AVCC ligada, Habilita canal 0
	ADCSRA = 0b11100111;				//Habilita AD, conversão contínua, prescaler = 128
	ADCSRB = 0x00;						//Modo de conversão contínua
	DIDR0 = 0b00111100;					//Pinos PC0 e PC1 como entrada ADC0 e ADC1

}

