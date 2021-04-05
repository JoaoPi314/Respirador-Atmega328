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

void changeDisplayConfig(uint8_t sel, uint8_t freq, uint32_t card, uint16_t sat, float temp){
    
    nokia_lcd_clear();
    nokia_lcd_plot(freq, "resp/min", 30);           //sempre chama a função de plot para atualizar o gráfico mesmo quando nã ofor mostrado

    if(!sel){                                       //Caso sel seja 0, plota o display simples
        nokia_lcd_clear();
        char freq_str[3];
        char card_srt[4];
        char sat_str[4];
        char temp_str[3];
        char temp_str_2[2];
        uint16_t temp_int = temp;
        uint8_t temp_dec = temp*10 - temp_int*10;
        
        nokia_lcd_set_cursor(35,0);
        nokia_lcd_write_string("resp/min", 1);      //Escreve as unidades em (35,14)
        nokia_lcd_set_cursor(35, 12);
        nokia_lcd_write_string("bpm", 1);
        nokia_lcd_set_cursor(35, 24);
        nokia_lcd_write_string("oC", 1);
        nokia_lcd_set_cursor(35, 36);
        nokia_lcd_write_string("\%SpO2", 1);


        itoa(freq, freq_str, 10);                   //Converte a frequência para string
        itoa(card, card_srt, 10);                   
        itoa(sat, sat_str, 10);
        itoa(temp_int, temp_str, 10);
        itoa(temp_dec, temp_str_2, 10);
        
        if(freq < 10){                              //Padding de 0 caso a frequência seja menor que 10
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string("0", 1);
        }
        else
            nokia_lcd_set_cursor(0, 0);
        
        nokia_lcd_write_string(freq_str, 1);        //Escreve a frequência em (0,0)
        nokia_lcd_set_cursor(0, 12);                
        nokia_lcd_write_string(card_srt, 1);        
        nokia_lcd_set_cursor(0, 24);
        nokia_lcd_write_string(temp_str, 1);
        nokia_lcd_write_string(".", 1);
        nokia_lcd_write_string(temp_str_2, 1);
        nokia_lcd_set_cursor(0, 36);
        nokia_lcd_write_string(sat_str, 1);       

    }

    nokia_lcd_render();

}

