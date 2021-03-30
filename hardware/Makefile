CC=avr-gcc
CO=avr-objcopy
CU=avrdude
CPU_F=16000000UL
BRD= atmega328p
BRDMOD= ATMEGA328P
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=main
HEX=main.hex
PORT=/dev/ttyACM0
BR=115200


all: main

main:
	make compile && make hex_compile && make upload


object_compile: $(SRC)
	for file in $^ ; do \
		$(CC) -Os -DF_CPU=$(CPU_F) -mmcu=$(BRD) -c -o $${file%.*}.o $${file};\
	done

compile : object_compile
	$(CC) -mmcu=$(BRD) $(OBJ) -o $(EXEC)

hex_compile:
	$(CO) -O ihex -R .eeprom $(EXEC) main.hex

upload:
	$(CU) -F -V -c arduino -p $(BRDMOD) -P $(PORT) -b $(BR) -U flash:w:$(HEX)

clean : mrproper
	rm -rf $(OBJ) $(HEX)

mrproper:
	rm -rf $(EXEC)
