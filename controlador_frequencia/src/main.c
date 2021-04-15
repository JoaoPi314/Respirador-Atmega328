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

//Variável estática só válida dentro desse arquivo

//Variáveis dos sensores
static volatile uint8_t FreqRespiracao = 12;		//Frequência de respiração
static volatile uint32_t FreqCardiaca = 60;			//Frequência cardíaca
static volatile uint16_t saturacaoO2 = 0;			//Saturação de O2 n sangue
static volatile float temper = 0;					//Temperatura
static volatile uint8_t pressureChar;				//Dado USART recebido
static volatile char pressure[8] = "       ";				//Pressão arterial
//Variável de controle principal
static volatile uint32_t tempo_ms = 0;				//Contador de tempo do programa

//Flags
static volatile uint8_t ledFlag = 0;				//Flag para habilitar animação dos LEDs
static volatile uint8_t flagLCD = 0;				//Flag para mostrar valores no LCD
static volatile uint8_t displayConfigFlag = 0;		//Flag que indica qual display será mostrado (geral ou gráfico)
static volatile uint8_t flagUsart = 0;				//Flag para indicar quando uma transação for recebida

static volatile char errorMSG[8] = "ERRO!  ";


typedef enum states{ERROR = 0b0000, //0
					INIT  = 0b0001, //1
					M1    = 0b0011, //3
					M2	  = 0b0010, //2
					M3    = 0b0110, //6
					X     = 0b0111, //7
					L1    = 0b0101, //5
					L2    = 0b0100, //4
					L3    = 0b1100,	//C
					LAST  = 0b1101, //D
					DONE  = 0b1001	//9
				} states;


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
states pressureMeasure(char *message);

//------------------------------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	gpioSetup();
	timerSetup();
	adcSetup();
	interruptSetup();
	usartSetup();

	initLCD();
	
    while (1){ 
  		
    	if(flagLCD){
  			changeDisplayConfig(displayConfigFlag, FreqRespiracao, FreqCardiaca, saturacaoO2, temper, (const char *)pressure);//Plota o gráfico da frequência x tempo e indica a frequência atual	
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


states pressureMeasure(char * message){

	static states actualState = INIT;
	static states nextState = INIT;

	static uint8_t i;
	actualState = nextState;

	//Processamento do próximo estado e da saída
	switch(actualState){
		case INIT:		//Estado idle, verifica o primeiro char
			nextState = (pressureChar == 0x3b) ? M1 : ERROR; 		//Só vai para o próximo estado caso o caractere seja ';'
			i = 0;
			break;

		case M1:		//Estado do primeiro dígito da máxima	
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? M2 : ERROR;	//Só vaí para M2 caso seja numérico
			message[i] = pressureChar;
			i++;
			break;

		case M2:		//Estado do segundo dígito da máxima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? M3 :		//Só vai para M3 caso seja numérico
						(pressureChar == 0x78) ? L1 : ERROR;							//Caso não, vai para L1 caso seja 'x'
			message[i] = pressureChar;
			i++;
			break;

		case M3:		//Estado do terceiro dígito da máxima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? X :		//Só vai para X caso seja numérico
						(pressureChar == 0x78) ? L1 : ERROR;							//Caso não, vai para L1 caso seja 'x'
			message[i] = pressureChar;
			i++;
			break;

		case X:			//Estado do caractere 'x' que divide a máxima e a mínima	
			nextState = (pressureChar == 0x78) ? L1 : ERROR;	//Só vai para L1 caso seja x
			message[i] = pressureChar;
			i++;
			break;

		case L1:		//Estado do primeiro dígito da mínima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? L2 : ERROR;		//Só vai para L2 caso seja numérico
			message[i] = pressureChar;
			i++;
			break;

		case L2:		//Estado do segundo dígito da mínima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? L3 :		//Só vai para L3 caso seja numérico
						(pressureChar == 0x3a) ? DONE : ERROR;						//Vai para DONE caso seja ':'
			message[i] = (nextState == L3) ? pressureChar : ' ';
			i++;
			break;

		case L3:		//Estado do terceiro dígito da mínima
			nextState = (pressureChar >= 0x30 && pressureChar <= 0x39) ? LAST : 	//Só vai para LAST caso seja numérico
						(pressureChar == 0x3a) ? DONE : ERROR;						//Só vai para DONE caso seja ':'
			message[i] = (nextState == LAST) ? pressureChar : ' ';
			i++;
			break;
		
		case LAST:
			nextState = (pressureChar == 0x3a) ? DONE : ERROR;						//Vai para DONE se ':'
			break;

		case DONE:		//Estado final
			nextState = (pressureChar == 0x3b) ? M1 : ERROR; 		//Só vai para M1 caso o caractere seja ';'
			i = 0;
			break;

		case ERROR:		//Estado padrão é ERROR
			nextState = (pressureChar == 0x3b) ? M1 : ERROR;		//Sai do erro com o ';'
			i = 0;
	}


	return nextState;
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
	pressureChar = UDR0;

	static char message[8] = "       "; 

	states decodeStatus = INIT;

	decodeStatus = pressureMeasure(message);
	
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