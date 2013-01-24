/* From http://www.instructables.com/files/orig/F79/38BJ/D48EV0FBBLL/F7938BJD48EV0FBBLL.c */

/* 
Playing with getting the small stepper motors driven.
*/

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

/* Prototypes */
void halfStepping(uint16_t delay, uint8_t direction[]);

int main(void){

  const uint8_t clockwise[] = {BROWN, YELLOW, BLACK, ORANGE, BROWN};
  const uint8_t counterClockwise[] = {ORANGE, BLACK, YELLOW, BROWN, ORANGE};
  uint8_t i;
   
  DDRB = 0xff; 			/* Enable output on all of the B pins */
  PORTB = 0x00;			/* Set them all to 0v */
  DDRD = 0xff;
  PORTD = 0x00;

  while(1){			/* main loop here */

    for (i=0; i<=20; i++){
      halfStepping(10, clockwise);    
    }   
    
    for (i=0; i<=20; i++){
      halfStepping(10, counterClockwise);    
    }   

  }
}


void halfStepping(uint16_t delay, uint8_t direction[]){
  uint8_t i;
  for ( i=0; i<=3; i++ ){	/* step through the phases */

    PORTB = direction[i];	/* single-coil part */
    PORTD = direction[i];
    _delay_ms(delay);
    
    PORTB |= direction[i+1];	/* add in half-step */
    PORTD |= direction[i+1];	/* add in half-step */
    _delay_ms(delay);
  }
}




