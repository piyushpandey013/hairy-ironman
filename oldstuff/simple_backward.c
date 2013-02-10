/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

/* Clockwise order */
#define BROWN _BV(PD0)
#define YELLOW _BV(PD1)
#define BLACK _BV(PD2)
#define ORANGE _BV(PD3)

int
main (void)
{
  const uint16_t delay = 10;
  DDRD = 0xff;			/* Enable output on all of the B pins */
  PORTD = 0x00;			/* Set them all to 0v */
  while (1)
    {				/* main loop here */
      PORTD = BROWN;
      _delay_ms (50);
      PORTD = ORANGE;
      _delay_ms (50);
      PORTD = BLACK;
      _delay_ms (50);
      PORTD = YELLOW;
      _delay_ms (50);
    }
}
