CC := gcc
AVR_CC := avr-gcc
CFLAGS := -std=c11 -g -Wall -Wextra -pedantic
PC_CFLAGS := -O0 -Isrc/Unity/src
AVR_CFLAGS := -mmcu=atmega328p -Os -DF_CPU=16000000ul

DEVICE ?= /dev/ttyACM0

.DEFAULT_GOAL = build/avr/main_avr.hex

build/avr/%.o:src/%.c
	mkdir -p $(dir $@)
	$(AVR_CC) $(CFLAGS) $(AVR_CFLAGS) -o $@ -c $^

build/pc/%.o:src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(PC_CFLAGS) -o $@ -c $^

build/avr/%.elf:
	mkdir -p $(dir $@)
	$(AVR_CC) $(CFLAGS) $(AVR_CFLAGS) -o $@ $^

build/pc/%.elf:
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(PC_CFLAGS) -o $@ $^

.PRECIOUS: build/avr/%.s
build/avr/%.s: build/avr/%.elf
	avr-objdump -d $< > $(basename $@).s

build/avr/%.hex: build/avr/%.elf build/avr/%.s
	avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: flash
flash: ${TARGET}
	avrdude -v -p m328p -c arduino -P ${DEVICE} -U flash:w:$<:i

src/%_runner.c: src/%.c
	ruby src/Unity/auto/generate_test_runner.rb $< $@

build/pc/results/%.txt: build/pc/%_test_runner.elf
	mkdir -p $(dir $@)
	-$< 2>&1 > $@

build/avr/blink.elf: build/avr/blink.o

build/avr/sender.elf: build/avr/sender.o build/avr/uart.o

build/avr/receiver.elf: build/avr/receiver.o build/avr/uart.o

AVR_SOURCES = $(addprefix src/,font.c indexmap.c screen.c mem.c tft.c fonts.c disp.c pin.c spi_sw.c panic_avr.c app.c)
AVR_OBJECTS = $(patsubst src/%,build/avr/%,$(patsubst %.c,%.o,${AVR_SOURCES}))

PC_SOURCES = $(addprefix src/,font.c indexmap.c screen.c mem.c tft.c fonts.c disp.c pin_pc.c spi_print.c panic_pc.c delay_pc.c)
PC_OBJECTS = $(patsubst src/%,build/pc/%,$(patsubst %.c,%.o,${PC_SOURCES}))

build/avr/main_avr.elf: build/avr/main_avr.o ${AVR_OBJECTS}

build/pc/main_pc.elf: build/pc/main_pc.o ${PC_OBJECTS}

build/pc/cpu_test_runner.elf: $(addprefix build/pc/,Unity/src/unity.o cpu_test.o cpu_test_runner.o cpu.o pipe_mock.o panic_pc.o)

build/pc/integration_test_runner.elf: $(addprefix build/pc/,Unity/src/unity.o integration_test.o grid.o integration_test_runner.o cpu.o pipe_mock.o panic_pc.o)
