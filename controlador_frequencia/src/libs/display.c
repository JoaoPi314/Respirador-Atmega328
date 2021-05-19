#include "display.h"

void show_vital(uint32_t card, uint16_t o2, float temp, char* pressure){

	ks0108_write_string("Dados vitais", 0, 0);

	for(uint8_t i = 0; i < 127; i+=2){
		ks0108_write(0x01, i, 1);
	}
	for(uint8_t i = 0; i < 8; i++){
		ks0108_write(0b01010101, 73, i);
	}

	char aux[5];
	char aux_2[2];
	uint8_t temper;
	int decimal_temper;

	temper = (int) temp;
	decimal_temper = (temp*10) - temper*10;

	itoa(card, aux, 10);
	strcat(aux, "  ");
	ks0108_write_string(aux, 0, 2);
	itoa(o2, aux, 10);
	strcat(aux, "  ");
	ks0108_write_string(aux, 0, 3);
	itoa(temp, aux, 10);
	itoa(decimal_temper, aux_2, 10);
	strcat(aux, ".");
	strcat(aux, aux_2);
	ks0108_write_string(aux, 0, 4);
	ks0108_write_string(pressure, 0, 5);

	ks0108_write_string("bpm", 43, 2);
	ks0108_write_string("\%SpO2", 43, 3);
	ks0108_write_string("oC", 43, 4);
	ks0108_write_string("mmHg", 43, 5);

}

void show_params(uint8_t freq, uint8_t o2Valv, uint8_t vol, uint8_t mode){

	ks0108_write_string("Params.", 85, 0);
	char aux[5];

	itoa(freq, aux, 10);
	strcat(aux, "  ");
	ks0108_write_string(aux, 76, 2);
	itoa(o2Valv, aux, 10);
	strcat(aux, "  ");
	ks0108_write_string(aux, 76, 3);
	itoa(vol, aux, 10);
	strcat(aux, "  ");
	ks0108_write_string(aux, 76, 5);
	if(!mode)
		ks0108_write_string("FORCED", 76, 6);
	else
		ks0108_write_string("ASSIST", 76, 6);

	ks0108_write_string("r/min", 99, 2);
	ks0108_write_string("\%", 99, 3);
	ks0108_write_string("vol", 99, 5);
}


void screen1(uint32_t card, uint16_t o2, float temp, char* pressure, uint8_t freq, uint8_t o2Valv, uint8_t vol, uint8_t mode){
	show_vital(card, o2, temp, pressure);
	show_params(freq, o2Valv, vol, mode);
	ks0108_write_string("           ", 2, 6);
	ks0108_write_string("           ", 2, 7);
}

void alert(uint8_t blink, uint8_t feature, uint8_t state){
	
	uint8_t col = 0;
	uint8_t aux = 0;
	if(blink){
		switch(feature){
			case 'T':
				col = 20;
				break;
			case 'C':
				col = 38;
				break;
			case 'O':
				col = 56;
		}

		switch(state){
			case 0:
				aux = 127;
				break;
			case 1:
				aux = 128;
				break;
		}

		ks0108_write_char(feature, col, 7);
		ks0108_write_char(aux, col+6, 7);

	}

}