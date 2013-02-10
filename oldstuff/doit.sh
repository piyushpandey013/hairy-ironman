#!/bin/bash

TARGET=$1
CODE=$2

avr-gcc -mmcu=$TARGET -Wall -Os -o $CODE.elf $CODE.c
avr-objcopy -j .text -j .data -O ihex $CODE.elf $CODE.hex -v
avrdude -c avr109 -P /dev/tty.usbmodemfd121 -p $TARGET -U flash:w:$CODE.hex
