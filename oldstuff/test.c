/* From http://www.instructables.com/id/EP4OIQNL5LEV0FB8YX/ */
/* Playing with getting the small stepper motors driven. */
/* Standard Includes */
// #include <inttypes.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #define F_CPU 20000000UL
// #include <util/delay.h>
#include <stdio.h>
#include <sys/types.h>

#define CW 0
#define CCW 1

//                                 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
const u_int8_t cw_half_bytes[]   = { 1,  3,  6,  2, 12,  0,  4,  0,  9,  1,  0,  0,  8 };
const u_int8_t ccw_half_bytes[]  = { 1,  9,  3,  1,  6,  0,  2,  0, 12,  8,  0,  0,  4 };
const u_int8_t cw_whole_bytes[]  = { 1,  2,  4,  6,  8,  0, 12,  0,  1,  3,  0,  0,  9 };
const u_int8_t ccw_whole_bytes[] = { 1,  8,  1,  9,  2,  0,  3,  0,  4, 12,  0,  0,  6 };

u_int8_t PORTB = 0x00;

void halfstep(u_int8_t dir)
{
  if (dir == CW)
  {
    PORTB = cw_half_bytes[PORTB];
  } else {
    PORTB = ccw_half_bytes[PORTB];
  }
}

void wholestep(u_int8_t dir)
{
  if (dir == CW)
  {
    PORTB = cw_whole_bytes[PORTB];
  } else {
    PORTB = ccw_whole_bytes[PORTB];
  }
}

int main()
{
  u_int8_t i = 0;
  for (i = 0; i < 4; i++)
  {
    wholestep(CW);
    printf("%s", PORTB);
  }
  printf(" | ");
  for (i = 0; i < 4; i++)
  {
    wholestep(CCW);
    printf("%s", PORTB);
  }
  return 0;
}