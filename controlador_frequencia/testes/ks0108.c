#include "ks0108.h"
#include "ASCIItable.h"

/*
 *
 *
 */

void ks0108_init(){
	set_CS1();
	set_CS2();
	reset_LCD();
	set_inst();
	set_write();
	DATA = POWER_LCD;	/////////////////////////////////////////////
	enable_LCD();
	DATA = ROW_INIT; //////////////////////////////////////////////
	enable_LCD();
	DATA = Y_INIT;   //////////////////////////////////////////////
	enable_LCD();
	DATA = PAG_INIT;/////////////////////////////////////////////
	enable_LCD();

	ks0108_clear();

}


/*
 *
 *
 */

void ks0108_write(uint8_t data, uint8_t col, uint8_t page){

	set_inst();

	if(col > 63){
		clr_CS1();
		set_CS2();
		DATA = Y_INIT + col - 64;
	}else{
		clr_CS2();
		set_CS1();
		DATA = Y_INIT + col;
	}

	enable_LCD();
	DATA = PAG_INIT + page;
	enable_LCD();

	set_data();
	DATA = data;
	enable_LCD();

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




