/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 20000000UL
#include <util/delay.h>
#include <avr/pgmspace.h> // for PROGMEM

int main()
{
  DDRD = 0xff;
  PORTD = 0x00;

  uint8_t i = 0;
  const uint8_t array[] PROGMEM = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  while (1)
  {
    PORTD = array[i];
//    PORTD = i;
    i++;
    _delay_ms(20);
  }
}