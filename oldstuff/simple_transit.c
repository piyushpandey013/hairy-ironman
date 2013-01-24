/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 20000000UL
#include <util/delay.h>

/* Clockwise order */
#define BROWN _BV(PB0)
#define YELLOW _BV(PB1)
#define BLACK _BV(PB2)
#define ORANGE _BV(PB3)

void testcall(uint16_t longdelay);

int
main (void)
{
  const uint16_t shortdelay = 50;
  const uint16_t longdelay = 500;
  DDRB = 0xff;			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  DDRD = 0xff;
  PORTD = 0x00;
  
  
  while (1)
    {				/* main loop here */
      PORTB = BROWN;
      PORTD = BROWN;
      _delay_ms (shortdelay);
      PORTB = YELLOW;
      PORTD = YELLOW;
      _delay_ms (shortdelay);
      PORTB = BLACK;
      PORTD = BLACK;
      _delay_ms (shortdelay);
      PORTB = ORANGE;
      PORTD = ORANGE;
      _delay_ms (shortdelay);
      PORTB = BROWN;
      PORTD = BROWN;
      _delay_ms (shortdelay);
      PORTB = YELLOW;
      PORTD = YELLOW;
      _delay_ms (shortdelay);
      PORTB = BLACK;
      PORTD = BLACK;
      _delay_ms (shortdelay);
      PORTB = ORANGE;
      PORTD = ORANGE;
      _delay_ms (longdelay);
      PORTB |= BLACK;
      PORTD |= BLACK;
      _delay_ms (longdelay);
      PORTB = BLACK;
      PORTD = BLACK;
      _delay_ms (longdelay);
      PORTB |= YELLOW;
      PORTD |= YELLOW;
      _delay_ms (longdelay);
      PORTB = YELLOW;
      PORTD = YELLOW;
      _delay_ms (longdelay);
      testcall(longdelay);
    }
}

void testcall(uint16_t longdelay)
{
        PORTB |= BROWN;
      PORTD |= BROWN;
      _delay_ms (longdelay);
}