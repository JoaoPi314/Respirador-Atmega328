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
#include "libs/registers.h"
#include "libs/pressureCtrl.h"

//Variável estática só válida dentro desse arquivo

//Variáveis dos sensores
static volatile uint8_t FreqRespiracao = 12;		//Frequência de respiração
static volatile uint32_t FreqCardiaca = 60;			//Frequência cardíaca
static volatile uint16_t saturacaoO2 = 0;			//Saturação de O2 n sangue
static volatile float temper = 10;					//Temperatura
static volatile char pressure[8] = "       ";				//Pressão arterial
//Variável de controle principal
static volatile uint32_t tempo_ms = 0;				//Contador de tempo do programa

//Flags
static volatile uint8_t ledFlag = 0;				//Flag para habilitar animação dos LEDs
static volatile uint8_t flagLCD = 0;				//Flag para mostrar valores no LCD
static volatile uint8_t displayConfigFlag = 0;		//Flag que indica qual display será mostrado (geral ou gráfico)

static volatile char errorMSG[8] = "ERRO!  ";




//Interrupções
ISR(ADC_vect);
ISR(TIMER0_COMPA_vect);
ISR(TIMER1_CAPT_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(PCINT2_vect);
ISR(USART_RX_vect);

//funções
void initLCD();
void ledRoutine();

//------------------------------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	uint8_t txAttribute = 0;		//0 = freqresp, 2 = temperatura, 3 = saturação de O2

	gpioSetup();
	timerSetup();
	adcSetup();
	interruptSetup();
	usartSetup();

	initLCD();
	
    while (1){ 
  		
    	if(flagLCD){
  			changeDisplayConfig(displayConfigFlag, FreqRespiracao, FreqCardiaca, saturacaoO2, temper, (const char *)pressure);//Plota o gráfico da frequência x tempo e indica a frequência atual	
    		
    		switch(txAttribute){
			case 0:
				UDR0 = FreqRespiracao;
				txAttribute++;
				break;
			case 1:
				UDR0 = (uint8_t) temper;
				txAttribute++;
				break;
			case 2:
				UDR0 = (uint8_t) saturacaoO2;
				txAttribute++;
				break;
			case 3:
				UDR0 = (uint8_t) FreqCardiaca;
				txAttribute++;
				break;
			default:
				for(uint8_t i = 0; i < 8; i++){
				 	while(!(UCSR0A & (1 << UDRE0)));
				 	UDR0 = pressure[i];
				}
			 	while(!(UCSR0A & (1 << UDRE0)));
			 	UDR0 = 100;
				txAttribute = 0;
				break;
			}

    		flagLCD = 0;

    	}
    	
    	if(ledFlag){
    		ledRoutine();
			ledFlag = 0;
		}
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------

void initLCD(){
	//LCD start
	nokia_lcd_init();					//Inicia o display LCD
	nokia_lcd_clear();					//Limpa a tela inicialmente
}

void ledRoutine(){

    uint8_t result = animateLed(FreqRespiracao);				//Chama rotina de animação de leds

	if(tst_bit(result, 0))
		set_bit(PORTD, 6);
	else
		clr_bit(PORTD, 6);

	clr_bit(result, 0);
	PORTB = result;
}



//------------------------------------------------------------------------------------------------------------------------------------------------------------

ISR(ADC_vect){
	clr_bit(ADCSRA, 3);							//ADC interrupt desabilitado
	
	//cpl_bit(PORTD, 0);						//Para debug da frequência que a interrupção é gerada (1/300m)Hz
	
	if(tst_bit(ADMUX, 0))						//Caso o canal 0 esteja sendo a fonte de interrupção
		saturacaoO2 = 0.123*ADC;				//Cálculo da reta da saturação de O2
	else
		temper = (ADC + 205	)/20.46;			//Canal 1 -> Cálculo da reta de temperatura

	//Alarme para condições críticas de temperatura e saturação de O2
	if((saturacaoO2 < 60) || (temper < 35) || (temper > 41)){
		if(!(tempo_ms % 300))				
			cpl_bit(PORTD, 5);
	}
	else
		clr_bit(PORTD, 5);


}


ISR(TIMER1_CAPT_vect){

	FreqCardiaca = (uint32_t)ICR1 * 64;
	FreqCardiaca /= 1000;
	FreqCardiaca = 60000/FreqCardiaca;

	TCNT1 = 0;

}

ISR(TIMER0_COMPA_vect){			//Interrupção por overflow do TC0
	tempo_ms++;					//Conta o tempo após 1ms
	

	if((tempo_ms % (60000/(FreqRespiracao*16))) == 0){		//Caso o tempo atinja 1/16 do período
		ledFlag = 1;										//Flag de animação dos LEDs ativa
	}

	if(!(tempo_ms % 150)){					//A cada 150ms o ADC é habilitado		
		set_bit(ADCSRA, 3);					//ADC interrupt habilitado
	}

	if(!(tempo_ms % 200)){					//A cada 200ms, mostra no LCD e altera o canal ADC
		flagLCD = 1;
		cpl_bit(ADMUX, 0);
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



ISR(USART_RX_vect){

	static char message[8] = "       "; 

	states decodeStatus = INIT;

	decodeStatus = pressureMeasure(message, UDR0);
	
	if(decodeStatus == ERROR){
		for(uint8_t j = 0; j < 8; j++){
			pressure[j] = errorMSG[j];
			message[j] = ' ';
		}
	}else if(decodeStatus == DONE){
	 	for(uint8_t j = 0; j < 7; j++){
	 		pressure[j] = message[j];
	 		message[j] = ' ';
	 	}

	}

}