/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

#define CW  0
#define CCW 1

#define MOTORPORT PORTB
// #define DEBUGPORT PORTD

//                                  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
const uint8_t cw_half_bytes[]   = { 1,  3,  6,  2, 12,  0,  4,  0,  9,  1,  0,  0,  8 };
const uint8_t ccw_half_bytes[]  = { 1,  9,  3,  1,  6,  0,  2,  0, 12,  8,  0,  0,  4 };
const uint8_t cw_whole_bytes[]  = { 1,  2,  4,  6,  8,  0, 12,  0,  1,  3,  0,  0,  9 };
const uint8_t ccw_whole_bytes[] = { 1,  8,  1,  9,  2,  0,  3,  0,  4, 12,  0,  0,  6 };

void halfstep(uint8_t dir)
{
  if (dir == CW)
  {
    MOTORPORT = cw_half_bytes[MOTORPORT];
    #ifdef DEBUGPORT
    DEBUGPORT = MOTORPORT;
    #endif
  } else {
    MOTORPORT = ccw_half_bytes[MOTORPORT];
    #ifdef DEBUGPORT
    DEBUGPORT = MOTORPORT;
    #endif
  }
}

void wholestep(uint8_t dir)
{
  if (dir == CW)
  {
    MOTORPORT = cw_whole_bytes[MOTORPORT];
    #ifdef DEBUGPORT
    DEBUGPORT = MOTORPORT;
    #endif
  } else {
    MOTORPORT = ccw_whole_bytes[MOTORPORT];
    #ifdef DEBUGPORT
    DEBUGPORT = MOTORPORT;
    #endif
  }
}

void doWholeSteps(uint8_t dir, uint16_t numsteps, uint16_t delay)
{
  uint16_t stepsrem;
  for (stepsrem = numsteps; stepsrem; stepsrem--)
  {
    wholestep(dir);
    _delay_us(delay);
  }
}

void doHalfSteps(uint8_t dir, uint16_t numsteps, uint16_t delay)
{
  uint16_t stepsrem;
  for (stepsrem = numsteps; stepsrem; stepsrem--)
  {
    halfstep(dir);
    _delay_us(delay);
  }
}

void initPorts()
{
  DDRB = 0xff;			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  DDRD = 0xff;
  PORTD = 0x00;
}

int main()
{
  
  initPorts();

  uint8_t i = 1;
  while (1)
  {
    doWholeSteps(CW, 100, 160000);
    doWholeSteps(CCW, 100, 160000);

  }
  
}
