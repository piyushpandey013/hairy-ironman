
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <util/delay.h>

#define MPIN1 0
#define MPIN2 1
#define MPIN3 2
#define MPIN4 3

#define MOTORPORT PORTB
#define DELAYUS 1000

#define CW 0
#define CCW 1

void doStep(uint8_t dir);

int main()
{
  DDRB = 0xff;
  PORTB = 0x00;
  
  uint16_t i = 0;
  while (1)
  {
    for (i = 0; i < 1800; i++)
    {
      doStep(CW);
      _delay_us(DELAYUS);
    }
    for (; i > 0; i--)
    {
      doStep(CCW);
      _delay_us(DELAYUS);
    }
  }
}

void doStep(uint8_t dir)
{
  switch ( MOTORPORT )
  {
    case (_BV(MPIN1) | _BV(MPIN4)):
      if (dir == CW)
        MOTORPORT =  _BV(MPIN1);
      else
        MOTORPORT = _BV(MPIN4);
      break;
    case (_BV(MPIN1)):
      if (dir == CW)
        MOTORPORT = _BV(MPIN1) | _BV(MPIN2) | _BV(MPIN3);
      else
        MOTORPORT = _BV(MPIN1) | _BV(MPIN4);
      break;
    case (_BV(MPIN1) | _BV(MPIN2) | _BV(MPIN3)):
      if (dir == CW)
        MOTORPORT = _BV(MPIN2) | _BV(MPIN3);
      else
        MOTORPORT = _BV(MPIN1);
      break;
    case (_BV(MPIN2) | _BV(MPIN3)):
      if (dir == CW)
        MOTORPORT = _BV(MPIN2) | _BV(MPIN3) | _BV(MPIN4);
      else
        MOTORPORT = _BV(MPIN1) | _BV(MPIN2) | _BV(MPIN3);
      break;
    case (_BV(MPIN2) | _BV(MPIN3) | _BV(MPIN4)):
      if (dir == CW)
        MOTORPORT = _BV(MPIN4);
      else
        MOTORPORT = _BV(MPIN2) | _BV(MPIN3);
      break;
    case (_BV(MPIN4)):
      if (dir == CW)
        MOTORPORT = _BV(MPIN1) | _BV(MPIN4);
      else
        MOTORPORT = _BV(MPIN2) | _BV(MPIN3) | _BV(MPIN4);
      break;
    default: // oops
      MOTORPORT = _BV(MPIN1) | _BV(MPIN4);
  }
}
