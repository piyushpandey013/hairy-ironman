/* From: http://www.instructables.com/id/EY8VBI7JT3EV0FB956/ */
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
  const uint16_t delay = 1000;
  const uint8_t clockwise[] = { BROWN, YELLOW, BLACK, ORANGE };
  uint8_t i;
  DDRB = 0xff;			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  DDRD = 0xff;
  PORTD = 0x00;
  while (1)
    {				/* main loop here */
      for (i = 0; i <= 3; i++)
	{			/* step through the colors clockwise */
	  PORTB = clockwise[i];
	  PORTD = clockwise[i];
	  _delay_ms (delay);
	}
      for (i = 3; i >= 0; i--)
	{			/* step through the colors ccw */
	  PORTB = clockwise[i];
	  PORTD = clockwise[i];
	  _delay_ms (delay);
	}
    }
}
