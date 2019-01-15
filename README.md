# TIS-100 for real

The goal of this project is to implement the computer in the brilliant TIS-100
game by Zachtronics on real hardware.

## Setup

    submodule update --init
    sudo apt install build-essential ruby

## Run tests on PC

    make build/pc/results/{cpu,integration}.txt

## Build and flash AVR binary

The code runs on an Arduino (or actually any ATmega328P) with a "red 128x160
ST7735S 1.8 inch display module" connected. Check main_avr.c for pinout.

    make build/avr/main_avr.elf               # Build only
    make flash TARGET=build/avr/main_avr.elf  # Build and flash
