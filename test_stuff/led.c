#include <avr/io.h>
#include <util/delay.h>

enum {
 BLINK_DELAY_MS = 1000,
};

int main (void)
{
 /* set pin 5 of PORTB for output*/
 DDRC |= _BV(DDC7);

 while(1) {
  /* set pin 5 high to turn led on */
  PORTC |= _BV(PORTC7);
  _delay_ms(BLINK_DELAY_MS);

  /* set pin 5 low to turn led off */
  PORTC &= ~_BV(PORTC7);
  _delay_ms(BLINK_DELAY_MS);
 }

 return 0;
}
