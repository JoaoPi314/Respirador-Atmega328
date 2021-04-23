compile:
	@avr-gcc -Wall -g -Os -Wl,-static -ffunction-sections -Wl,--gc-sections -mmcu=atmega328p -o \
	../outputs/main.bin ../src/libs/moveServo.c ../src/libs/nokia5110.c ../src/libs/nokiaDisplay.c \
	../src/libs/registers.c ../src/libs/pressureCtrl.c ../src/main.c
	@avr-objcopy -j .text -j .data -O ihex ../outputs/main.bin ../outputs/main.hex
	@avr-size -C ../outputs/main.bin --mcu=atmega328p
	

config_ports:
	@sudo tty_bus -s /tmp/ttyS4mux & sleep 1
	@sudo tty_attach -s /tmp/ttyS4mux /dev/ttyS4 & sleep 1
	@sudo tty_fake -s /tmp/ttyS4mux /dev/ttyS4fake0 & \
		tty_fake -s /tmp/ttyS4mux /dev/ttyS4fake1 & sleep 1

view_data:
	@python3 ../data_visualizer/serialCom.py
