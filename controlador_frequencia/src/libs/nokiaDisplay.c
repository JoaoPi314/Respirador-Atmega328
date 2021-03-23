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

void changeDisplayConfig(uint8_t sel, uint8_t freq){

    nokia_lcd_clear();

    switch(sel){

        char freq_str[3];

        case 0:
            nokia_lcd_plot(freq, "resp/min", 30);
            break;
        default:
	        nokia_lcd_set_cursor(35,14);
            nokia_lcd_write_string("resp/min", 1);
            
            itoa(freq, freq_str, 10);
    	    
            
    	    if(freq < 10){
    		    nokia_lcd_set_cursor(0, 0);
    		    nokia_lcd_write_string("0", 3);
			    nokia_lcd_set_cursor(16,0);
        	}
		    else
			    nokia_lcd_set_cursor(0, 0);
		    
            nokia_lcd_write_string(freq_str, 3);

            break;
    }

    nokia_lcd_render();

}

