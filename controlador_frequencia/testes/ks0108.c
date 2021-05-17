#include "ks0108.h"
#include "ASCIItable.h"

/*
 *
 *
 */



void ks0108_init(){
	static uint8_t ctrl_byte = 0b00000000;
	static uint8_t data_byte = 0b00000000;

	set_CS1(ctrl_byte);
	set_CS2(ctrl_byte);
	reset_LCD(ctrl_byte);
	set_inst(ctrl_byte);
	set_write(ctrl_byte);
	data_byte = POWER_LCD;
	serial_com(ctrl_byte, data_byte);
	enable_LCD();

	data_byte = ROW_INIT;
	serial_com(ctrl_byte, data_byte);
	enable_LCD();
	data_byte = Y_INIT;   
	serial_com(ctrl_byte, data_byte);
	enable_LCD();

	data_byte = PAG_INIT;

	serial_com(ctrl_byte, data_byte);
	enable_LCD();
	ks0108_clear();

	set_bit(PORTC, 0);
}


/*
 *
 *
 */

void ks0108_write(uint8_t data, uint8_t col, uint8_t page){
	static uint8_t ctrl_byte = 0b00000000;
	static uint8_t data_byte = 0b00000000;

	set_inst(ctrl_byte);

	if(col > 63){
		clr_CS1(ctrl_byte);
		set_CS2(ctrl_byte);
		data_byte = Y_INIT + col - 64;
	}else{
		clr_CS2(ctrl_byte);
		set_CS1(ctrl_byte);
		data_byte = Y_INIT + col;
	}

	serial_com(ctrl_byte, data_byte);
	enable_LCD();

	data_byte = PAG_INIT + page;
	serial_com(ctrl_byte, data_byte);
	enable_LCD();

	set_data(ctrl_byte);
	data_byte = data;
	enable_LCD();
	serial_com(ctrl_byte, data_byte);

}

/*
 *
 *
 */

void ks0108_clear(){
	
	uint8_t page, col;

	for(page = 0; page < 9; page++){
		for(col = 0; col < 128; col++)
			ks0108_write(0x00, col, page);
	}
}

/*
 *
 *
 */

void ks0108_write_char(uint8_t c, uint8_t col, uint8_t page){
	uint8_t i = 0;

	for(i = 0; i < 5; i++){
		ks0108_write(ASCII_table[c - 32][i], col+i, page);
	}

}


void serial_com(uint8_t ctrl, uint8_t data){

	uint16_t word = 0xffff;//((uint16_t)data << 8) | ctrl;
	uint8_t i = 8;
	do{
		i--;
		if(tst_bit(word, i))
			set_bit(CTRL_PORT, DT);
		else
			clr_bit(CTRL_PORT, DT);

		CLK_pulse();

	}while(i!= 0);


	STB_pulse();
}