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
	DDRD |= 0b01100011;					//Aciona os pinos 2, 3 e 7 da porta D como entrada
	
	//Inicialização e pull-ups
	PORTD |= 0b10001100;				//Aciona o pull-up interno para os pinos 2, 3 e 7 da porta D
	PORTB |= 0b00000001;				//Inicialmente todas as saídas estão NLB em B. Pull up ativo em B0
}



void timerSetup(){
	//Configuração dos timers
	//TC0
	TCCR0A = 0b00000010;				//TC0 operando em modo CTC
	TCCR0B = 0b00000011;				//Liga TC0 com prescaler = 64
	OCR0A  = 249;						//TC0 conta até 249


	//TC1
	ICR1   = 39999;						//Frequência do PWM em 50Hz nessa configuração 
	TCCR1A = 0b10100010;				//Canais OC1A e OC1B com PWM rápido~(TOP = ICR1)
	TCCR1B = 0b00011010;				//Prescaler em 1, Comparação com ICR1
	OCR1A  = 2000;
	OCR1B  = 2000;

}


void interruptSetup(){
	//Interrupções externas
	EICRA = 0b00001010;					//Interrupções INT1 e INT0 ativadas na borda de descida
	EIMSK =	0b00000011;					//Ativa as interrupções INT1 e INT0
	PCICR  = 0b00000101;				//Interrupções por mudança na porta D e B ativadas

	PCMSK0 = 0b00000001;				//Ativa a interrupção individual do pino PB0
	PCMSK2 = 0b00010000;				//Ativa a interrupção individual do pino PD4

	//Timers
	TIMSK0 = 0b00000010;				//Habilita interrupção por comparação com OCR0A

	sei();								//Bit SREG em 1 - Interrupções globais ativadas
}


void adcSetup(){
	//configuração ADC
	ADMUX = 0b01000001;					//AVCC ligada, Habilita canal 0
	ADCSRA = 0b11100111;				//Habilita AD, conversão contínua, prescaler = 128
	ADCSRB = 0x00;						//Modo de conversão contínua
	DIDR0 = 0b00111100;					//Pinos PC0 e PC1 como entrada ADC0 e ADC1

}

void usartSetup(){
	//Configuração USART
	UCSR0B = 0b10011000;					//Interrupções por RX habilitadas, receiver habilitado, transmiter habilitado
	UCSR0C = 0b00000110;					//Modo assíncrono, sem paridade, 1 bit de parada, 8 bits de dado

	UBRR0H = ((MYUBRR) >> 8);					//Parte mais significativa do Baud rate
	UBRR0L = MYUBRR;						//Parte menos significativa do Baud rate
}