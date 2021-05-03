/*---------------------------------------------------------------------	*
 * Arquivo: nokiaDisplay.c  											*
 * Criador: João Pedro Melquiades Gomes      Mat: 118110077				*
 * Descrição: Arquivo com definição da função que controla o que será   *
 * mostrado no display													*
 *---------------------------------------------------------------------	*/

#include "nokiaDisplay.h"

/*
 * A função changedisplayConfig() recebe como parâmetro o sel.
 * Caso 0, mostra apenas o display básico, caso contrário, mostra o gráfico
 * da frequência de respiração em função do tempo
 */

void changeDisplayConfig(uint8_t sel, uint8_t freq, uint32_t card, uint16_t sat, float temp, const char* pressure, uint8_t o2Valv, uint8_t volume){
    
    nokia_lcd_clear();

    char freq_str[3];
    char card_srt[8];
    char sat_str[4];
    char temp_str[3];
    char temp_str_2[2];
    char o2_str[4];
    char volume_str[3];
    uint16_t temp_int = temp;
    uint8_t temp_dec = temp*10 - temp_int*10;



    for(uint8_t i = 0; i < 84; i++)
        nokia_lcd_set_pixel(i, 9, 1);

    switch(sel){
        case 0:             //Display dos dados vitais

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
            nokia_lcd_write_char('*', 1);
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

            break;

        case 2:
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string(" Parametros", 1);

            itoa(freq, freq_str, 10);                   
            itoa(o2Valv, o2_str, 10);
            itoa(volume, volume_str, 10);

            nokia_lcd_set_cursor(2, 24);
            nokia_lcd_write_char('*', 1);
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

            break;
        case 3:
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string(" Parametros", 1);

            itoa(freq, freq_str, 10);                   
            itoa(o2Valv, o2_str, 10);
            itoa(volume, volume_str, 10);

            nokia_lcd_set_cursor(2, 36);
            nokia_lcd_write_char('*', 1);
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

    }
    
    nokia_lcd_render();

}

