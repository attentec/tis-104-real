DEVICE ?= /dev/ttyACM0

.DEFAULT_GOAL := ${TARGET}

avr/%.o:%.c
	mkdir -p $(dir $@)
	avr-gcc -o $@ -c $^ \
		-std=c11 -mmcu=atmega328p -g -Os -DF_CPU=16000000ul

pc/%.o:%.c
	mkdir -p $(dir $@)
	gcc -o $@ -c $^ \
		-std=c11 -g -Os

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

CHD_SOURCES = font.c indexmap.c screen.c mem.c
CHD_OBJECTS = $(addprefix avr/,$(patsubst %.c,%.o,${CHD_SOURCES}))

CHD_PC_SOURCES = font.c indexmap.c screen.c mem.c
CHD_PC_OBJECTS = $(addprefix pc/,$(patsubst %.c,%.o,${CHD_PC_SOURCES}))

TFT_SOURCES = tft.c fonts.c disp.c pin.c spi_sw.c panic.c
TFT_OBJECTS = $(addprefix avr/,$(patsubst %.c,%.o,${TFT_SOURCES}))

TFT_PC_SOURCES = tft.c fonts.c disp.c pin_pc.c spi_pc.c panic.c delay_pc.c
TFT_PC_OBJECTS = $(addprefix pc/,$(patsubst %.c,%.o,${TFT_PC_SOURCES}))

avr/display.elf: avr/display.o ${TFT_OBJECTS} ${CHD_OBJECTS}

pc/display: pc/display.o ${TFT_PC_OBJECTS} ${CHD_PC_OBJECTS}
