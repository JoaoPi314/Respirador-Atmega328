compile:
	@avr-gcc -Wall -g -Os -mmcu=atmega328p -o ../outputs/main.bin ../src/libs/myDelay.c ../src/libs/animateLed.c ../src/main.c
	@avr-objcopy -j .text -j .data -O ihex ../outputs/main.bin ../outputs/main.hex
	@avr-size -C ../outputs/main.bin --mcu=atmega328p