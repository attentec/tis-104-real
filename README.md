# TIS-100 for real

The goal of this project is to implement the computer in the brilliant TIS-100
game by Zachtronics on real hardware.

## Setup

    submodule update --init
    sudo apt install build-essential gcc-avr binutils-avr avr-libc avrdude ruby
    # install TUP: http://gittup.org/tup/

## Build everything

    tup

## Run tests on PC

Build everything and run tests:

    ./test

## Build and flash AVR binary

The code runs on an Arduino (or actually any ATmega328P) with a "red 128x160
ST7735S 1.8 inch display module" connected. Check main_avr.c for pinout.

Build everything and flash:

    ./flash main_avr

The DEVICE environment variable can be used to override the Arduino USB device
used. Any extra arguments are passed to Avrdude.
