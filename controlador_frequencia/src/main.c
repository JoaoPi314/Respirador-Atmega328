/*---------------------------------------------------------------------	*
 * Arquivo: main.c														*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com conjunto de instruções que irão para o ATmega	*
 *---------------------------------------------------------------------	*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "libs/moveServo.h"
#include "libs/display.h"
#include "libs/mascaras.h"
#include "libs/registers.h"
#include "libs/pressureCtrl.h"

//Variável estática só válida dentro desse arquivo

//Controle dos atuadores
static volatile uint8_t volumeRespirador = 10;		//Controle do volume do respirador


//Variáveis dos sensores
static volatile uint8_t FreqRespiracao = 12;		//Frequência de respiração
static volatile uint32_t FreqCardiaca = 60;			//Frequência cardíaca
static volatile uint16_t saturacaoO2 = 0;			//Saturação de O2 n sangue
static volatile float temper = 10;					//Temperatura
static volatile char pressure[8] = "       ";		//Pressão arterial
//Variável de controle principal
static volatile uint32_t tempo_ms = 0;				//Contador de tempo do programa

//Flags
static volatile uint8_t diafrag = 0;				//Flag que indica se houve movimento no diafragma
static volatile uint8_t enable = 0;					//Flag que habilita o servo para respiração
static volatile uint8_t segFlag = 0;				//Flag que conta os segundos
static volatile uint8_t breathMode = 0;				// 0 - Forçado; 1 - Assistido
static volatile uint8_t servoFlag = 0;				//Flag para habilitar animação dos LEDs
static volatile uint8_t flagLCD = 0;				//Flag para mostrar valores no LCD
static volatile uint8_t displayConfigFlag = 0;		//Flag que indica qual display será mostrado (geral ou gráfico)
static volatile uint8_t requestFromAlert = 0;		//Flag que indica que o buzzer está sendo usado pelo mecanismo de alerta

static volatile char errorMSG[8] = "ERRO!  ";

static time total;									//Tempo total de funcionamento do respirador


//Interrupções
ISR(ADC_vect);
ISR(TIMER2_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(PCINT2_vect);
ISR(USART_RX_vect);
ISR(TIMER1_CAPT_vect);
ISR(PCINT0_vect);

//------------------------------------------------------------------------------------------------------------------------------------------------------
void cronometer();
//------------------------------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
	uint8_t txAttribute = 0;		//0 = freqresp, 2 = temperatura, 3 = saturação de O2

	gpioSetup();
	timerSetup();
	adcSetup();
	interruptSetup();
	usartSetup();

	nokia_lcd_init();
	nokia_lcd_clear();
    while (1){ 

    	if(flagLCD){
  			changeDisplayConfig(displayConfigFlag, FreqRespiracao, FreqCardiaca, saturacaoO2, temper, (const char *)pressure, 10*OCR0A - 120, volumeRespirador, breathMode, total);//Plota o gráfico da frequência x tempo e indica a frequência atual	
    		
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
				 	while(!(UCSR0A & (1 << UDRE0)));	//Infelizmente a CPU trava um pouco aqui, mas não observei nenhum resultado negativo devido a isso
				 	UDR0 = pressure[i];
				}
			 	while(!(UCSR0A & (1 << UDRE0)));
			 	UDR0 = 100;
				txAttribute = 0;
				break;
			}

    		flagLCD = 0;

    	}
    	if(enable){			//Só movimenta o servo caso esteja a menos que 5cm do sonar
			if(!requestFromAlert){
				if(OCR0B < 13){
					set_bit(PORTB, 6);
				}
				else{
					clr_bit(PORTB, 6);
				}
			}
	    	if(servoFlag){		//Só entra caso esteja no modo forçado ou sem movimentação no digragma (modo Smart)
	    		OCR0B = moveServo(volumeRespirador);				//Chama rotina que move o servo
				servoFlag = 0;
			}
			
    	}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------

ISR(ADC_vect){
	clr_bit(ADCSRA, 3);							//ADC interrupt desabilitado
	
	
	if(!tst_bit(ADMUX, 2) && !tst_bit(ADMUX, 1) && !tst_bit(ADMUX, 0))						//Caso o canal 0 esteja sendo a fonte de interrupção
		saturacaoO2 = 0.123*ADC;				//Cálculo da reta da saturação de O2
	else if(!tst_bit(ADMUX, 2) && !tst_bit(ADMUX, 1) && tst_bit(ADMUX, 0))					//Caso o canal 1 esteja sendo a fonte de interrupção
		temper = (ADC + 205	)/20.46;
	else if(!tst_bit(ADMUX, 2) && tst_bit(ADMUX, 1) && !tst_bit(ADMUX, 0)){			//Caso o canal 2 esteja sendo a fonte de interrupção
		switch(ADC){
			case 168:	//Seleção de tela
				if(displayConfigFlag < 2)		//Vai alterando o display a cada aperto, varia de 0 a 2
					displayConfigFlag++;
				else
					displayConfigFlag = 0;
				break;
			case 157:	//Muda modo
				if(breathMode < 2)
					breathMode++;
				else
					breathMode = 0;
					if(breathMode != 1)
						diafrag = 0;
					else
						diafrag = 1;
				break;
			case 960:	// Diminui freq. resp
				if(FreqRespiracao > 5)
					FreqRespiracao--;
				break;
			case 675:	//Aumenta freq. resp
				if(FreqRespiracao < 30)
					FreqRespiracao++;
				break;
			case 852:	// Diminui O2
				if(OCR0A > 12)
					OCR0A -= 1;	//Cada 1 no duty cycle significa menos 10%
				break;
			case 619:	//Aumenta O2
				if(OCR0A < 22)
					OCR0A += 1;	//Cada 1 no duty cycle significa mais 10%			
				break;
			case 401:	//Diminui vol
				if(volumeRespirador > 1)
					volumeRespirador --;
				break;
			case 340:	//Aumenta vol
				if(volumeRespirador < 10)
					volumeRespirador ++;
				break;
			
		}

	}

	//Alarme para condições críticas de temperatura e saturação de O2
	if((saturacaoO2 < 60) || (temper < 35) || (temper > 40) || (FreqCardiaca < 50) || (FreqCardiaca > 140)){				
		requestFromAlert = 1;
		set_bit(PORTB, 6);
	}
	else if(requestFromAlert){
		clr_bit(PORTB, 6);
		requestFromAlert = 0;
	}

	
	if(!tst_bit(ADMUX, 1))	//Caso nao esteja ainda no canal 3
		ADMUX ++;							//altera o canal ADC após feita uma leitura
	else
		ADMUX-=2;
}


ISR(TIMER2_COMPA_vect){			//Interrupção por overflow do TC0
	tempo_ms++;					//Conta o tempo após 1ms

	if(!breathMode || !diafrag)		//No breath mode 0, a respiração é controlada totalmente pelo hardware
		if((tempo_ms % (60000/(FreqRespiracao*2*volumeRespirador))) == 0){//Caso o tempo atinja 1/(volume*2)do período
			servoFlag = 1;										//Flag de animação dos LEDs ativa
		}

	if(!(tempo_ms % 50)){					//A cada 50ms o ADC é habilitado		
		set_bit(ADCSRA, 3);					//ADC interrupt habilitado
	}

	if(!(tempo_ms % 200)){					//A cada 200ms, mostra no LCD
		flagLCD = 1;
	}

	if(!(tempo_ms % 100)){					//A cada 100ms, o sonar faz uma medição de distância
		trigger();
	}

	if(!(tempo_ms % 1000))					//Conta q segundo no cronômetro
		cronometer();

}


ISR(PCINT2_vect){				//Interrupção externa na porta D

	static uint32_t lastTime = 0;

	if(tst_bit(PIND, 7)){						//Caso PD4 tenha sido NLA
		lastTime = tempo_ms;					//Armazena o tempo que estava NLA
	}
	else if(!(tst_bit(PIND, 7))){				//Caso PD4 tenha sido NLB
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
ISR(TIMER1_CAPT_vect){

	static uint32_t start;			//Tempo inicial da medição
	static uint32_t distance = 20;	//tempo final da medição

	cpl_bit(TCCR1B, ICES1);			//Após uma medição no posedge,a próxima é no negedge

	if(!tst_bit(TCCR1B, ICES1))		//Se a captura foi em um posedge
		start = ICR1;
	else if(ICR1 > start){			//caso a segunda medição seja maior que a primeira, calcule o período (Evita overflow)
		distance = (ICR1 - start)/116;
	}

	if((distance < 5) && (distance > 0)){	//Ativa o servo se a distância for menor que 5cm
		enable = 1;
	}
	else{
		enable = 0;
		clr_bit(PORTB, 6);					//Caso o alarme esteja ativo, desativa se o enable desligar
	}

}

ISR(PCINT0_vect){
	if(enable){				//Só move o servo caso sonar esteja a menos de 5cm 
		if(breathMode == 1 || diafrag){			//Caso modo assistido ou haja movimento no diafragma no modo smart
			if(tst_bit(PINB, 7)){				
				OCR0B = 12 + volumeRespirador;
				diafrag = 1;
			}else if(!tst_bit(PINB, 7)){
				OCR0B = 12;
			}
		}else if(breathMode == 2){				//Se o modo for smart e houve movimento no diafragma
			if(tst_bit(PINB, 7)){
				diafrag = 1;
			}
		}
	}
}

void cronometer(){

	//Lógica simples para contar segundos
	total.seg++;
	if(total.seg == 60){
		total.seg = 0;
		total.min++;
	}
	if(total.min == 60){
		total.hour++;
		total.min = 0;
	}

}