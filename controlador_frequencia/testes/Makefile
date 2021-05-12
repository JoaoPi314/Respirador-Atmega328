compile:
	@avr-gcc -Wall -g -Os -Wl,-static -ffunction-sections -Wl,--gc-sections -mmcu=atmega328p -o \
	main.bin ks0108.c main.c
	@avr-objcopy -j .text -j .data -O ihex main.bin main.hex
	@avr-size -C main.bin --mcu=atmega328p
	

config_ports:
	@sudo tty_bus -s /tmp/ttyS4mux & sleep 1
	@sudo tty_attach -s /tmp/ttyS4mux /dev/ttyS4 & sleep 1
	@sudo tty_fake -s /tmp/ttyS4mux /dev/ttyS4fake0 & \
		tty_fake -s /tmp/ttyS4mux /dev/ttyS4fake1 & sleep 1

view_data:
	@python3 ../data_visualizer/serialCom.py
