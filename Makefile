TARGET = stepper
LIBS = 
#CC = c99
CC = avr-gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Werror -O2
PART = atmega32u4
#PART = atmega324p
ARCH = -mmcu=$(PART)
OBJCOPY = avr-objcopy
PROG = avrdude
PROGCMD = -U flash:w:$(TARGET).hex
PROGDEV = /dev/tty.usbmodemfd121
PROGFLAGS = -c avr109 -P $(PROGDEV) -p $(PART)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(ARCH) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(ARCH) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET) $(TARGET).hex

hex: $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET) $(TARGET).hex

burn: hex
	$(PROG) $(PROGFLAGS) $(PROGCMD)
