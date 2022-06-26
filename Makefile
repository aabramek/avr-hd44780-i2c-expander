CC=avr-gcc.exe
CSTD=c99
OPTIMIZATION=-O2
WARN=-Wall

FCPU=1000000
MMCU=atmega32a

CFLAGS=$(WARN) $(OPTIMIZATION) -mmcu=$(MMCU) -DF_CPU=$(FCPU) -Iinclude -std=$(CSTD)

LIB=bin/lcd.a

CFILES=$(subst src/,,$(wildcard src/*.c))
OBJFILES=$(patsubst %.c,%.o,$(CFILES))


.PHONY: all
all: $(LIB)

$(LIB): obj/$(OBJFILES)
	mkdir bin; avr-ar rs $@ $^

obj/%.o: src/%.c
	mkdir obj; $(CC) $(CFLAGS) -c -o $@ $^
	
.PHONY: clean
clean:
	rm -rf obj bin
