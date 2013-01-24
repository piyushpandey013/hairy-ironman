#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/sfr_defs.h>

#define ADCPORT PORTA
#define DEBUGPORT PORTD

int main()
{
  DDRB = 0xff;			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  DDRD = 0xff;
  PORTD = 0x00;

// set ADC voltage reference to AVCC w/ external cap at AREF
  ADMUX = _BV(REFS0) | _BV(ADLAR);
// note that ADMUX:MUX4..0 are zero to select ADC0 (PORTA:PIN0)
// enable ADC, trigger first conversion to initialize ADC, and set sample rate to 125 kHz
  ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1);

  while (1)
  {
    // trigger ADC sample
    ADCSRA |= _BV(ADSC);
    // poll delay until sample done
    loop_until_bit_is_clear( ADCSRA, ADSC );
    // get and store sample value
    DEBUGPORT = 255 >> (8-((ADCH+31)/32));
  }
}
