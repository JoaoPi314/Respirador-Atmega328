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
	DDRC |= 0b00000000;
	DDRB |= 0b01111110;					//Todos os pinos da porta B serão de saída, exceto B0, B6 e B7
	DDRD |= 0b01101111;					//Aciona os pinos 2, 3 e 4 da porta D como entrada
	
	//Inicialização e pull-ups
	PORTD |= 0b00000000;				//Aciona o pull-up interno para os pinos 2 e 3 da porta D
	PORTB |= 0b00000001;				//Inicialmente todas as saídas estão NLB em B. Pull up ativo em B0 e B6
}



void timerSetup(){
	//Configuração dos timers
	//TC2
	TCCR2A = 0b00000010;				//TC2 operando em modo CTC
	TCCR2B = 0b00000100;				//Liga TC2 com prescaler = 8
	OCR2A  = 249;						//TC2 conta até 249 


	//TC1
	TCCR1B = (1<<ICES1)|(1<<CS11);
	//ICR1   = 39999;						//Frequência do PWM em 50Hz nessa configuração 
	//TCCR1A = 0b10100010;				//Canais OC1A e OC1B com PWM rápido~(TOP = ICR1)
	//TCCR1B = 0b00011010;				//Prescaler em 1, Comparação com ICR1
	//OCR1A  = 2000;
	//OCR1B  = 2000;


	//TC0
	TCCR0A = 0b10100011;			//Modo PWM rápido
	TCCR0B = 0b00000111;			//Clock externo em T0
	OCR0A = 12;
	OCR0B = 12;
	//TCCR0B = 0b00000111;	
}	


void interruptSetup(){
	//Interrupções externas
	//EICRA = 0b00001010;					//Interrupções INT1 e INT0 ativadas na borda de descida
	//EIMSK =	0b00000011;					//Ativa as interrupções INT1 e INT0
	PCICR  = 0b00000101;				//Interrupções por mudança na porta D e B ativadas

	PCMSK0 = 0b10000000;				//Ativa a interrupção individual do pino PB7
	PCMSK2 = 0b10000000;				//Ativa a interrupção individual do pino PD7

	//Timers
	//TIMSK0 = 0b00000001;
	TIMSK1 = 1 << ICIE1;
	TIMSK2 = 0b00000010;				//Habilita interrupção por comparação com OCR0A

	sei();								//Bit SREG em 1 - Interrupções globais ativadas
}


void adcSetup(){
	//configuração ADC
	ADMUX = 0b01000001;					//AVCC ligada, Habilita canal 0
	ADCSRA = 0b11100111;				//Habilita AD, conversão contínua, prescaler = 128
	ADCSRB = 0x00;						//Modo de conversão contínua
	DIDR0 = 0b00111000;					//Pinos PC0, PC1 e PC2 como entrada ADC0, ADC1 e ADC2

}

void usartSetup(){
	//Configuração USART
	UCSR0B = 0b10011000;					//Interrupções por RX habilitadas, receiver habilitado, transmiter habilitado
	UCSR0C = 0b00000110;					//Modo assíncrono, sem paridade, 1 bit de parada, 8 bits de dado

	UBRR0H = ((MYUBRR) >> 8);				//Parte mais significativa do Baud rate
	UBRR0L = MYUBRR;						//Parte menos significativa do Baud rate
}