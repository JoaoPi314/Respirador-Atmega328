#define F_CPU 16000000UL //Clock de 16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "libs/mascaras.h"
#include "libs/nokia5110.h"

static volatile uint16_t temper_int = 0;
static volatile uint8_t temper_frac = 0;
static volatile uint32_t tempo_ms = 0;
static volatile float temper = 0.0;

ISR(ADC_vect);
ISR(TIMER0_COMPA_vect);
float piControler(float error);



int main(){
	char teste[6];
	float dutyPWM = 0;
	
	//GPIO
	DDRB |= 0xFF;		//Porta B como saída
	PORTB = 0x00;		//Todos os pinos de B em 0

	DDRC = 0xFE;		//Porta C como saída exceto PC0
	PORTC = 0x00;		//PC0 sem pull-up e resto em 0V


	//Configuração de timers
	TCCR0A = 0b00000010;				//TC0 operando em modo CTC
	TCCR0B = 0b00000011;				//Liga TC0 com prescaler = 64
	OCR0A  = 249;						//TC0 conta até 249
	TIMSK0 = 0b00000010;				//Habilita interrupção por comparação com OCR0A

	TCCR1B = 0b01000101;				//Prescaler em 1024, captura no posedge

	//PWM

	TCCR2A = 0b10000011;				//Habilita PWM Rápido não invertido no pino 0C2A (PB3)
	TCCR2B = 0x01;




	set_bit(TIMSK1, ICIE1);				//Habilita interrupção por captura

	//Interrupções por ADC
	ADMUX = 0b11000000;					//Aref ligada, Habilita canal 0
	ADCSRA = 0b11101111;				//Habilita AD, habilita interrupção, conversão contínua,
										//prescaler = 128
	ADCSRB = 0x00;						//Modo de conversão contínua
	DIDR0 = 0b00111110;					//Pinos PC0 como entrada ADC0

	sei();
	nokia_lcd_init();

	while(1){

		dutyPWM = piControler(36.0 - temper);

		OCR2A = dutyPWM*2.55;

		nokia_lcd_clear();
		itoa(temper_int, teste, 10);
		nokia_lcd_write_string(teste, 1);
		nokia_lcd_write_string(".", 1);
		itoa(temper_frac, teste, 10);
		nokia_lcd_write_string(teste, 1);
		nokia_lcd_render();
	}

}

ISR(ADC_vect){					//Interrupção a cada 150ms
	clr_bit(ADCSRA, 7);
	clr_bit(ADCSRA, 6);
	clr_bit(ADCSRA, 5);
	clr_bit(ADCSRA, 3);
	temper = ADC*0.108;
	temper_int = ADC*0.108;
	temper_frac = ADC*10.8 - temper_int*100;

}

ISR(TIMER0_COMPA_vect){			//Interrupção por overflow do TC0 (1ms)
	tempo_ms++;
	if(!(tempo_ms % 150)){
		set_bit(ADCSRA, 7);
		set_bit(ADCSRA, 6);
		set_bit(ADCSRA, 5);
		set_bit(ADCSRA, 3);
	}
}


float piControler(float error){		//A entrada do controlador PI é sempre 36 - temp(t)

	static float sumError = 0;
	float kp = 10, ki = -100;			//Constantes de proporcionalidade e de integração
	float output = 0;

	//Primeiro passo

	output = kp * error;

	//Segundo passo

	sumError+=error;
	output = ki * sumError;
	return output;

}