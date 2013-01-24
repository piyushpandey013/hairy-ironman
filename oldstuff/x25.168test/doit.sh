#!/bin/bash

TARGET=$1
CODE=$2

avr-gcc -mmcu=$TARGET -Wall -Os -o $CODE.elf $CODE.c
avr-objcopy -j .text -j .data -O ihex $CODE.elf $CODE.hex
avrdude -c stk500v2 -P /dev/ttyUSB0 -p $TARGET -U flash:w:$CODE.hex
