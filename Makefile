DEVICE ?= /dev/ttyACM0

.DEFAULT_GOAL := out/${TARGET}.hex

out/%.o:%.c
	mkdir -p $(dir $@)
	avr-gcc -o $@ -c $^ \
		-std=c11 -mmcu=atmega328p -g -Os -DF_CPU=16000000ul

%.elf:
	mkdir -p $(dir $@)
	avr-gcc -o $@ $^ \
		-std=c11 -mmcu=atmega328p -g -Os -DF_CPU=16000000ul
	avr-objdump -d $@ > $(basename $@).s

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: flash
flash: out/${TARGET}.hex
	avrdude -v -p m328p -c arduino -P ${DEVICE} -U flash:w:$<:i

out/blink.elf: out/blink.o

out/sender.elf: out/sender.o out/uart.o

out/receiver.elf: out/receiver.o out/uart.o
