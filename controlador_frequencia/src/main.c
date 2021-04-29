/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com conjunto de instruções que irão para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/moveServo.h"
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
static volatile char pressure[8] = "       ";		//Pressão arterial
//Variável de controle principal
static volatile uint32_t tempo_ms = 0;				//Contador de tempo do programa

//Flags
static volatile uint8_t servoFlag = 0;				//Flag para habilitar animação dos LEDs
static volatile uint8_t flagLCD = 0;				//Flag para mostrar valores no LCD
static volatile uint8_t displayConfigFlag = 0;		//Flag que indica qual display será mostrado (geral ou gráfico)
static volatile uint8_t requestFromAlert = 0;		//Flag que indica que o buzzer está sendo usado pelo mecanismo de alerta

static volatile char errorMSG[8] = "ERRO!  ";




//Interrupções
ISR(ADC_vect);
ISR(TIMER2_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(PCINT2_vect);
ISR(PCINT0_vect);
ISR(USART_RX_vect);

//funções
void initLCD();

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
  			changeDisplayConfig(displayConfigFlag, FreqRespiracao, FreqCardiaca, saturacaoO2, temper, (const char *)pressure, (OCR1B/20) - 100);//Plota o gráfico da frequência x tempo e indica a frequência atual	
    		

    		//A cada 200ms, um dos 5 dados é enviado para o LCD
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
				 	while(!(UCSR0A & (1 << UDRE0)));	//Infelizmente a CPU é trava um pouco aqui, mas não observei nenhum resultado negativo devido a isso
				 	UDR0 = pressure[i];
				}
			 	while(!(UCSR0A & (1 << UDRE0)));
			 	UDR0 = 100;
				txAttribute = 0;
				break;
			}

    		flagLCD = 0;

    	}
    	
    	if(servoFlag){
    		OCR1A = moveServo(FreqRespiracao);				//Chama rotina de animação de leds
			servoFlag = 0;
			if(!requestFromAlert){
				if(OCR1A == 2000){
					set_bit(PORTD, 6);
				}
				else{
					clr_bit(PORTD, 6);
				}
			}
		}
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------

void initLCD(){
	//LCD start
	nokia_lcd_init();					//Inicia o display LCD
	nokia_lcd_clear();					//Limpa a tela inicialmente
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------

ISR(ADC_vect){
	clr_bit(ADCSRA, 3);							//ADC interrupt desabilitado
	
	
	if(tst_bit(ADMUX, 0))						//Caso o canal 0 esteja sendo a fonte de interrupção
		saturacaoO2 = 0.123*ADC;				//Cálculo da reta da saturação de O2
	else
		temper = (ADC + 205	)/20.46;			//Canal 1 -> Cálculo da reta de temperatura

	//Alarme para condições críticas de temperatura e saturação de O2
	if((saturacaoO2 < 60) || (temper < 35) || (temper > 41)){				
		requestFromAlert = 1;
		if((tempo_ms % 300))
			cpl_bit(PORTD, 6);
	}
	else{
		clr_bit(PORTD, 6);
		requestFromAlert = 0;
	}

	cpl_bit(ADMUX, 0);							//altera o canal ADC após feita uma leitura
}


ISR(TIMER2_COMPA_vect){			//Interrupção por overflow do TC0
	tempo_ms++;					//Conta o tempo após 1ms


	if((tempo_ms % (60000/(FreqRespiracao*18))) == 0){		//Caso o tempo atinja 1/18 do período
		servoFlag = 1;										//Flag de animação dos LEDs ativa
	}

	if(!(tempo_ms % 150)){					//A cada 150ms o ADC é habilitado		
		set_bit(ADCSRA, 3);					//ADC interrupt habilitado
	}

	if(!(tempo_ms % 200)){					//A cada 200ms, mostra no LCD
		flagLCD = 1;
	}


}

ISR(INT0_vect){					//Interrupção externa em PD2

	switch(displayConfigFlag){
		case 0:	//Configuração padrão, apenas mostra os dados vitais
			break;
		case 1:	//Ajuste da frequência de respiração
			if(FreqRespiracao < 30)
				FreqRespiracao++;
			OCR0A = 915.527/FreqRespiracao - 1;
			OCR0B = OCR0A / 2;

			break;
		case 2: //Ajuste da válvula de O2
			if(OCR1B < 4000)
				OCR1B += 200;	//Cada 200 no duty cycle significa mais 10%
		default:
			break;
	}
}

ISR(INT1_vect){					//Interrupção externa em PD3

	switch(displayConfigFlag){
		case 0:	//Configuração padrão, apenas mostra os dados vitais
			break;
		case 1:	//Ajuste da frequência de respiração
			if(FreqRespiracao > 5)
				FreqRespiracao--;
			OCR0A = 915.527/FreqRespiracao - 1;
			OCR0B = OCR0A / 2;
			break;
		case 2: //Ajuste da válvula de O2
			if(OCR1B > 2000)
				OCR1B -= 200;	//Cada 200 no duty cycle significa mais 10%
		default:
			break;
	}
}


ISR(PCINT0_vect){				//Interrupção externa na porta B
	if(!tst_bit(PINB, 0)){				//Caso PB0 tenha sido a causa da interrupção, mude o display
		if(displayConfigFlag < 2)		//Vai alterando o display a cada aperto, varia de 0 a 2
			displayConfigFlag++;
		else
			displayConfigFlag = 0;
	}	
}


ISR(PCINT2_vect){				//Interrupção externa na porta D

	static uint32_t lastTime = 0;

	if(tst_bit(PIND, 4)){						//Caso PD4 tenha sido NLA
		lastTime = tempo_ms;					//Armazena o tempo que estava NLA
	}
	else if(!(tst_bit(PIND, 4))){				//Caso PD4 tenha sido NLB
		if(tempo_ms > lastTime){
			FreqCardiaca = 2*(tempo_ms - lastTime);	//Freq tem o período em ms
			FreqCardiaca = 60000 / FreqCardiaca;	//Frq tem a frequência em bpm
		}
	}
}



ISR(USART_RX_vect){

	static char message[8] = "       "; 		//Mensagem que armazenará o valor da pressão

	states decodeStatus = INIT;					//Estado inicial da FSM

	decodeStatus = pressureMeasure(message, UDR0);	//Chamada da FSM
	
	if(decodeStatus == ERROR){					//Caso o estado retornado seja o de erro, pressure = "ERRO!  "
		for(uint8_t j = 0; j < 8; j++){
			pressure[j] = errorMSG[j];
			message[j] = ' ';
		}
	}else if(decodeStatus == DONE){				//Caso o estado retornado seja o DONE, pressure = message
	 	for(uint8_t j = 0; j < 7; j++){
	 		pressure[j] = message[j];
	 		message[j] = ' ';
	 	}

	}

}