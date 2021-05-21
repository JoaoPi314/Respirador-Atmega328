/*---------------------------------------------------------------------	*
 * Arquivo: display.c		  											*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com definição da função que controla o que será   *
 * mostrado no display													*
 *---------------------------------------------------------------------	*/

#include "display.h"

/*
 * Função: changeDisplayConfig()
 * Descrição: Atualiza o display nokia de acordo com o que deve ser mostrado 
 * Parâmetros:  @sel: Flag que indica qual display será mostrado
				@freq: Frequência respiratória em rsp/min
				@card: Frequência cardíaca em bpm
				@sat: Saturação de O2 em %SpO2
				@temp: Temperatura em °C
				@pressure: Pressão arterial em mmHg
				@o2Valv: Estado da válvula de O2
				@volume: Volume de ar a ser injetado em cada respiração (Varia de 1 a 10)
				@breathMode: Modo de operação do respirador (Forçado = 0, assistido = 1 e smart = 2)
				@exec_time: Struct com horas, minutos e segundos
 */

void changeDisplayConfig(uint8_t sel, uint8_t freq, uint32_t card, uint16_t sat, float temp, const char* pressure, uint8_t o2Valv, uint8_t volume, uint8_t breathMode, time exec_time){
    
    nokia_lcd_clear();
	
	//Strings auxiliares para conversão de valores
    char freq_str[3];
    char card_srt[8];					
    char sat_str[4];
    char temp_str[3];
    char temp_str_2[2];
    char o2_str[4];
    char volume_str[3];
    char aux[3];
    char mode;
    
	//Conversão da temperatura em float para inteiro + uma casa decimal
    uint16_t temp_int = temp;	
    uint8_t temp_dec = temp*10 - temp_int*10;
	

	//Indicação do modo na tela
    if(!breathMode)
        mode = 'F';
    else if(breathMode == 1)
        mode = 'A';
    else
    	mode = 'S';

	//Desenho de uma linha para separar o título do resto do display
    for(uint8_t i = 0; i < 84; i++)
        nokia_lcd_set_pixel(i, 9, 1);

    switch(sel){
        case 0:             //Display dos dados vitais

			//Verificação de casos de alarme, desenha uma seta pra cima ou pra baixo se houver algum dado vital
			//alto ou baixo, respectivamente.
        	if(card < 50){
        		nokia_lcd_set_cursor(40, 12);
        		nokia_lcd_write_char(0x80, 1);
        	}else if(card > 140){
        		nokia_lcd_set_cursor(40, 12);
        		nokia_lcd_write_char(0x81, 1);
        	}

        	if(sat < 60){
        		nokia_lcd_set_cursor(40, 31);
        		nokia_lcd_write_char(0x80, 1);
        	}

        	if(temp < 35){
        		nokia_lcd_set_cursor(40, 21);
        		nokia_lcd_write_char(0x80, 1);
        	}else if(temp > 40){
        		nokia_lcd_set_cursor(40, 21);
        		nokia_lcd_write_char(0x81, 1);
        	}


            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string(" Dados vitais", 1);

            itoa(card, card_srt, 10);                   
            itoa(sat, sat_str, 10);
            itoa(temp_int, temp_str, 10);
            itoa(temp_dec, temp_str_2, 10);
            
            nokia_lcd_set_cursor(2, 12);
            nokia_lcd_write_string(card_srt, 1);        
            nokia_lcd_set_cursor(2, 21);
            nokia_lcd_write_string(temp_str, 1);
            nokia_lcd_write_string(".", 1);
            nokia_lcd_write_string(temp_str_2, 1);
            nokia_lcd_set_cursor(2, 31);
            nokia_lcd_write_string(sat_str, 1); 
            nokia_lcd_set_cursor(2, 41);
            nokia_lcd_write_string(pressure, 1);      

            nokia_lcd_set_cursor(50, 12);
            nokia_lcd_write_string("bpm", 1);        
            nokia_lcd_set_cursor(50, 21);
            nokia_lcd_write_string("oC", 1);
            nokia_lcd_set_cursor(50, 31);
            nokia_lcd_write_string("\%SpO2", 1); 
            nokia_lcd_set_cursor(50, 41);
            nokia_lcd_write_string("mmHg", 1);      
            break;

        case 1:             //Display dos parâmetros (1)
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string(" Parametros", 1);

            itoa(freq, freq_str, 10);                   
            itoa(o2Valv, o2_str, 10);
            itoa(volume, volume_str, 10);


            nokia_lcd_set_cursor(2, 12);
            nokia_lcd_set_cursor(10, 12);
            nokia_lcd_write_string(freq_str, 1);
            nokia_lcd_set_cursor(10, 24);
            nokia_lcd_write_string(o2_str, 1);
            nokia_lcd_set_cursor(10, 36);
            nokia_lcd_write_string(volume_str, 1);          
            
            nokia_lcd_set_cursor(36, 12);
            nokia_lcd_write_string("resp/min", 1);
            nokia_lcd_set_cursor(36, 24);
            nokia_lcd_write_string("\%O2", 1);
            nokia_lcd_set_cursor(36, 36);
            nokia_lcd_write_string("vol", 1);

            nokia_lcd_set_cursor(60, 24);
            nokia_lcd_write_char(mode, 3);

            break;

       	case 2:				//Display do cronômetro
       		nokia_lcd_clear();
       		nokia_lcd_set_cursor(12, 16);
       		itoa(exec_time.hour, aux, 10);
       		nokia_lcd_write_string(aux, 2);
       		nokia_lcd_write_char(':', 2);
    		itoa(exec_time.min, aux, 10);
       		nokia_lcd_write_string(aux, 2);
       		nokia_lcd_write_char(':', 2);
       		itoa(exec_time.seg, aux, 10);
       		nokia_lcd_write_string(aux, 2);
    }
    
    nokia_lcd_render();

}