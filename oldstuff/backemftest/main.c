/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/sfr_defs.h>

#define CW  0
#define CCW 1

#define ADCPORT PORTA
#define MOTORPORT PORTB
#define DEBUGPORT PORTD

// this is too magical :(
//                                  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
const uint8_t cw_half_bytes[]   = { 1,  3,  6,  2, 12,  0,  4,  0,  9,  1,  0,  0,  8 };
const uint8_t ccw_half_bytes[]  = { 1,  9,  3,  1,  6,  0,  2,  0, 12,  8,  0,  0,  4 };

//                                0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
const uint8_t adc_cw_bytes[]  = { 7,  1,  2,  7,  3,  7,  7,  7,  0,  7,  7,  7,  7 };
const uint8_t adc_ccw_bytes[] = { 7,  3,  0,  7,  1,  7,  7,  7,  2,  7,  7,  7,  7 };

uint8_t adctested = 0;
uint8_t stepperVal = 0;

void halfStep(uint8_t dir)
{
  if (dir == CW)
  {
    MOTORPORT = cw_half_bytes[MOTORPORT];
  } else {
    MOTORPORT = ccw_half_bytes[MOTORPORT];
  }
}

void initPorts()
{
  DDRB = 0xff;			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  DDRD = 0xff;
  PORTD = 0x00;
}

void initADC()
{
  // set ADC voltage reference to AVCC w/ external cap at AREF
  ADMUX = _BV(REFS0) | _BV(ADLAR);
  // set ADC multiplexer to "test" value (GND) for first initialization
  ADMUX |= _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
  // enable ADC, trigger first conversion to initialize ADC, and set sample rate to 125 kHz
  ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1);

  // it might be useful to test the result of the ADC here 
  //  to make sure it agrees with the test value (0V)

}

int main()
{

  uint8_t dir = CW;
  
  initPorts();
  initADC();

// accel section
  halfStep(dir);
  _delay_us(10000);
  halfStep(dir);
  _delay_us(6000);
  halfStep(dir);
  _delay_us(4667);
  halfStep(dir);
  _delay_us(3949);

  while (1)
  {
    stepperVal = MOTORPORT;
    switch (stepperVal)
    {
      case 1:
	ADMUX = (ADMUX & 0b1110000) | 0x1;
	adctested = 1;
	break;
      case 2:
	ADMUX = (ADMUX & 0b1110000) | 0x2;
	adctested = 1;
	break;
      case 4:
	ADMUX = (ADMUX & 0b1110000) | 0x3;
	adctested = 1;
	break;
      case 8:
	ADMUX = (ADMUX & 0b1110000) | 0x0;
	adctested = 1;
	break;
      default:
	ADMUX = (ADMUX & 0b1110000) | 0x31; // GND
	adctested = 0;
    }


    if (adctested)
    {
      // trigger ADC sample
      ADCSRA |= _BV(ADSC);
      // poll delay until sample done
      loop_until_bit_is_clear( ADCSRA, ADSC );
      DEBUGPORT = ADCH;
    }
    halfStep(dir);
    _delay_us(3500);

  } // main loop
  
}
