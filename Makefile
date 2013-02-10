TARGET = stepper
LIBS = 
#CC = c99
CC = avr-gcc
CFLAGS = -std=c99 -gstabs -Wall -Wextra -Werror -O2
PART = atmega32u4
#PART = atmega328
ARCH = -mmcu=$(PART)
OBJCOPY = avr-objcopy
PROG = avrdude
PROGCMD = -U flash:w:$(TARGET).hex
PROGDEV = /dev/tty.usbmodemfd121
PROGFLAGS = -c avr109 -P $(PROGDEV) -p $(PART)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.c, %.o, platform/$(PART).c)
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(ARCH) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(ARCH) $(CFLAGS) -Wall $(LIBS) -o $@

clean:
	-rm -rf *.o
	-rm -f $(TARGET) $(TARGET).hex

hex: $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET) $(TARGET).hex

burn: hex
	$(PROG) $(PROGFLAGS) $(PROGCMD)
