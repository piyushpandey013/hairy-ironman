
#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{

  // set data direction
  DDRB = 0xFF;
  PORTB = 0x00;
  DDRD = 0xFF;
  PORTD = 0x00;

  TCCR0 |= _BV(WGM01); // set timer0 to CTC mode
  TIMSK |= _BV(OCIE0); // enable compare match interupt
  OCR0 = 0xFF; // set the compare register to maximum time

  TCNT0 = 0x00; // reset the counter
  TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00); // start the timer (@ f_clk/1024)

  sei();

  while (1);

}

ISR(TIMER0_COMP_vect)
{
  PORTB = PORTB++;
  PORTD = PORTD++;
}
