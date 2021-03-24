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

void changeDisplayConfig(uint8_t sel, uint8_t freq, uint32_t card){
    
    nokia_lcd_clear();
    nokia_lcd_plot(freq, "resp/min", 30);           //sempre chama a função de plot para atualizar o gráfico mesmo quando nã ofor mostrado

    if(!sel){                                       //Caso sel seja 0, plota o display simples
        nokia_lcd_clear();
        char freq_str[3];
        char card_srt[6];
        
        nokia_lcd_set_cursor(35,14);
        nokia_lcd_write_string("resp/min", 1);      //Escreve as unidades em (35,14)
        nokia_lcd_set_cursor(35, 34);
        nokia_lcd_write_string("bpm", 1);


        itoa(freq, freq_str, 10);                   //Converte a frequência para string
        
        
        if(freq < 10){                              //Padding de 0 caso a frequência seja menor que 10
            nokia_lcd_set_cursor(0, 0);
            nokia_lcd_write_string("0", 3);
            nokia_lcd_set_cursor(16,0);
        }
        else
            nokia_lcd_set_cursor(0, 0);
        
        nokia_lcd_write_string(freq_str, 3);        //Escreve a frequência em (0,0)
        nokia_lcd_set_cursor(0, 30);                
        itoa(card, card_srt, 10);                   
        nokia_lcd_write_string(card_srt, 2);        


    }

    nokia_lcd_render();

}

