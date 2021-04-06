#define F_CPU 16000000UL //Clock de 16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "libs/mascaras.h"
#include "libs/nokia5110.h"

static volatile uint32_t temper = 0;
static volatile uint32_t tempo_ms = 0;
static volatile uint8_t piFlag = 0;

ISR(ADC_vect);
ISR(TIMER0_COMPA_vect);
float piControler(float error);



int main(){
	char pwm_str[4];
	uint16_t dutyPWM = 0;
	
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
	TCCR2B = 0x01;						//Pre-scaler em 1




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


		if(piFlag){									//A cada 1 segundo faz o controle
			dutyPWM = piControler(340 - temper);	//340 ~ 36°C
			piFlag = 0;								//Limpa a flag
		}

		OCR2A = dutyPWM;							//Registrador de PWM recebe a saída do controlador PI

		nokia_lcd_clear();
		itoa(dutyPWM, pwm_str, 10);
		nokia_lcd_write_string(pwm_str, 1);
		nokia_lcd_render();
	}

}

ISR(ADC_vect){					//Interrupção a cada 150ms
	clr_bit(ADCSRA, 7);
	clr_bit(ADCSRA, 6);
	clr_bit(ADCSRA, 5);
	clr_bit(ADCSRA, 3);
	temper = ADC;

}

ISR(TIMER0_COMPA_vect){			//Interrupção por overflow do TC0 (1ms)
	tempo_ms++;
	if(!(tempo_ms % 150)){
		set_bit(ADCSRA, 7);
		set_bit(ADCSRA, 6);
		set_bit(ADCSRA, 5);
		set_bit(ADCSRA, 3);
	}

	if(!(tempo_ms % 1000))
		piFlag = 1;

}


float piControler(float error){		//A entrada do controlador PI é sempre 36 - temp(t)

	float prop, integ = 0;
	float kp = 400;
	float ki = 100;
	uint16_t PI;

	prop = kp * error;			//Fator proporcional
	integ += ki * error;		//Fator integrativo

	PI = prop + integ;			//PI

	
	PI /= 256;					//Resolução de 16 bits para 8 bits do pwm
	if(PI == 256) PI = 255;		//Evita estouro

	return PI + 30;				//Necessário para convergir para aproximadamente 36°C

}