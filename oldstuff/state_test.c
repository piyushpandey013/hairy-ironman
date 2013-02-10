/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>

/* Clockwise order */
#define BROWN (1<<PORTD0)
#define YELLOW (1<<PORTD1)
#define BLACK (1<<PORTD2)
#define ORANGE (1<<PORTD3)

uint_fast8_t motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
uint_fast8_t motorStates = 6;

int
main (void)
{
    DDRD = 0xff;            /* Enable output on all of the B pins */
    PORTD = 0x00;            /* Set them all to 0v */
    uint_fast8_t curr = 0;
    while (1)
    {
        /* main loop here */
        int i;
        while (++i < 255)
        {
            PORTD = motorStateMap[curr];
            curr = (curr + 5) % motorStates;
            _delay_ms(25);
        }
        while (--i > 0)
        {
            PORTD = motorStateMap[curr];
            curr = (curr + 1) % motorStates;
            _delay_ms(25);
        }
    }
}
