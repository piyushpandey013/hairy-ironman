/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 12000000UL
#include <util/delay.h>

/* Clockwise order */
#define BROWN _BV(PB0)
#define YELLOW _BV(PB1)
#define BLACK _BV(PB2)
#define ORANGE _BV(PB3)

int
main (void)
{
  const uint16_t delay = 500;
  DDRB = 0xff;			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  uint8_t i = 0;
  while (1) {
    for (i = 0; i <= 10; i++) {
      PORTB = BROWN;
      _delay_ms (delay);
      PORTB |= YELLOW;	/* add in half-step */
      _delay_ms (delay);
      PORTB = YELLOW;
      _delay_ms (delay);
      PORTB |= BLACK;	/* add in half-step */
      _delay_ms (delay);
      PORTB = BLACK;
      _delay_ms (delay);
      PORTB |= ORANGE;	/* add in half-step */
      _delay_ms (delay);
      PORTB = ORANGE;
      _delay_ms (delay);
      PORTB |= BROWN;	/* add in half-step */
      _delay_ms (delay);
    }
    for (i = 0; i <= 10; i++) {
      PORTB = BROWN;
      _delay_ms (delay);
      _delay_ms (delay);
      PORTB = YELLOW;
      _delay_ms (delay);
      _delay_ms (delay);
      PORTB = BLACK;
      _delay_ms (delay);
      _delay_ms (delay);
      PORTB = ORANGE;
      _delay_ms (delay);
      _delay_ms (delay);
    }
  }
}