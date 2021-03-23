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

changeDisplayConfig(uint8_t sel, uint8_t Freq){

    switch(sel){
        case 0:
            nokia_lcd_plot(Freq, "resp/min", 30);
            break;
        default:
            break;
    }

    nokia_lcd_render();

}

