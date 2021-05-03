CC=avr-gcc.exe
CFLAGS=-Wall -O2 -mmcu=atmega8a -DF_CPU=1000000UL -I./headers
OUTPUT=bin/program.hex

vpath %.c source
vpath %.h headers
vpath %.o obj

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o obj/$@

compile: main.o lcd.o
	$(CC) $(CFLAGS) $^ -o $(OUTPUT)

upload: $(OUTPUT)
	avrdude.exe -p m8 -c usbasp -U flash:w:$(OUTPUT)