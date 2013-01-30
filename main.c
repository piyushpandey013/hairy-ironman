
#include <stdbool.h>

#define F_CPU 16000000L

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "controller.h"
#include "platform.h"

void blinky(void);

int main()
{
usb_workaround();

    init_controller(&SControl);
//  initInput();

    blinky();

    sei(); // enable global interrupts

    // Just move the gauge back and forth, for now
    while (true)
    {
        set_gauge_target( &SControl, (15<<4) );
        while (SControl.current_pos != 100) { controller_thread(&SControl); }
        blinky();
        //set_gauge_target( &SControl, 0 );
        //while (SControl.current_pos != 0) { controller_thread(&SControl); }
        //blinky();
    }
  
    blinky();

    /*
    while (true)
    {
        controller_thread();
        input_thread();
    }
*/
    return 0;
}

void blinky(void)
{
    // testing
    for (int i = 5; i > 0; i -= 1)
    {
        PORTC ^= (1<<PORTC7);
        for (int j = 1; j <= i; j += 1)
            _delay_ms(100);
        PORTC ^= (1<<PORTC7);
        for (int j = 1; j <= i; j += 1)
            _delay_ms(100);
    }
}

ISR(BADISR_vect)
{
    PORTC |= (1<<PORTC7);
    _delay_ms(1000);
    PORTC ^= (1<<PORTC7);
    for (int i = 3; i > 0; i--)
    {
        PORTC ^= (1<<PORTC7);
        _delay_ms(150);
        PORTC ^= (1<<PORTC7);
        _delay_ms(150);
    }    for (int i = 3; i > 0; i--)
    {
        PORTC ^= (1<<PORTC7);
        _delay_ms(300);
        PORTC ^= (1<<PORTC7);
        _delay_ms(300);
    }    for (int i = 3; i > 0; i--)
    {
        PORTC ^= (1<<PORTC7);
        _delay_ms(150);
        PORTC ^= (1<<PORTC7);
        _delay_ms(150);
    }
}
