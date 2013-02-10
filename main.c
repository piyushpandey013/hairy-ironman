
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

    while (true)
    {
    for (int i = 0; i < 900; i++)
    {
        advance_motor(&(SControl.MControl), UP);
        _delay_us(1200);
    }
    for (int i = 0; i < 900; i++)
    {
        advance_motor(&(SControl.MControl), DOWN);
        _delay_us(1200);
    }
    }
    blinky();
  
    return 0;
}

void blinky(void)
{
    // testing
    for (int i = 5; i > 0; i -= 1)
    {
        platform_light_status_led();
        for (int j = 1; j <= i; j += 1)
            _delay_ms(50);
        platform_toggle_status_led();
        for (int j = 1; j <= i; j += 1)
            _delay_ms(50);
    }
}

ISR(BADISR_vect)
{
    platform_light_status_led();
    _delay_ms(1000);
    platform_toggle_status_led();
    for (int i = 3; i > 0; i--)
    {
        platform_toggle_status_led();
        _delay_ms(150);
        platform_toggle_status_led();
        _delay_ms(150);
    }    for (int i = 3; i > 0; i--)
    {
        platform_toggle_status_led();
        _delay_ms(300);
        platform_toggle_status_led();
        _delay_ms(300);
    }    for (int i = 3; i > 0; i--)
    {
        platform_toggle_status_led();
        _delay_ms(150);
        platform_toggle_status_led();
        _delay_ms(150);
    }
}
