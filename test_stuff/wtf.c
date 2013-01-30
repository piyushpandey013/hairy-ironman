
#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t val = 65000;

void init_timer()
{
    TCCR1B |= (1<<WGM12);
    OCR1A = val;
}

void start_timer()
{
    TCCR1B |= (1<<CS11) | (1<<CS10);
    TIMSK1 |= (1<<OCIE1A); // enable output compare matching
}

void main()
{
    DDRC |= (1<<DDC7) | (1<<DDC6);
    DDRB |= (1<<DDB6);

    init_timer();

    start_timer();

    while (true) ;

}

ISR(TIMER1_COMPA_vect)
{
    PORTC ^= (1<<PORTC7);
}

ISR(TIMER1_OVF_vect)
{
    PORTC ^= (1<<PORTC6);
}

ISR(BADISR_vect)
{
    PORTB |= (1<<PORTB6);
    PORTC |= (1<<PORTC7) | (1<<PORTC6);
}
