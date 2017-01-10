DEVICE ?= /dev/ttyACM0

.DEFAULT_GOAL = avr/main_avr.hex

avr/%.o:%.c
	mkdir -p $(dir $@)
	avr-gcc -o $@ -c $^ \
		-std=c11 -mmcu=atmega328p -g -Os -DF_CPU=16000000ul \
		-Wall -Wextra -pedantic

pc/%.o:%.c
	mkdir -p $(dir $@)
	gcc -o $@ -c $^ \
		-std=c11 -g -Os \
		-Wall -Wextra -pedantic

avr/%.elf:
	mkdir -p $(dir $@)
	avr-gcc -o $@ $^ \
		-std=c11 -mmcu=atmega328p -g -Os -DF_CPU=16000000ul

.PRECIOUS: avr/%.s
avr/%.s: avr/%.elf
	avr-objdump -d $< > $(basename $@).s

avr/%.hex: avr/%.elf avr/%.s
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: flash
flash: ${TARGET}
	avrdude -v -p m328p -c arduino -P ${DEVICE} -U flash:w:$<:i

avr/blink.elf: avr/blink.o

avr/sender.elf: avr/sender.o avr/uart.o

avr/receiver.elf: avr/receiver.o avr/uart.o

AVR_SOURCES = font.c indexmap.c screen.c mem.c tft.c fonts.c disp.c pin.c spi_sw.c panic.c app.c
AVR_OBJECTS = $(addprefix avr/,$(patsubst %.c,%.o,${AVR_SOURCES}))

PC_SOURCES = font.c indexmap.c screen.c mem.c tft.c fonts.c disp.c pin_pc.c spi_pc.c panic.c delay_pc.c
PC_OBJECTS = $(addprefix pc/,$(patsubst %.c,%.o,${PC_SOURCES}))

avr/main_avr.elf: avr/main_avr.o ${AVR_OBJECTS}

pc/main_pc.elf: pc/main_pc.o ${PC_OBJECTS}
