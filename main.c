
#include <stdbool.h>

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "controller.h"
#include "platform.h"

void blinky(void);

int main()
{
usb_workaround(); // because the arduino bootloader is a piece of shit

    init_controller(&SControl);

    blinky();

// zero the motor
    for (int i = 0; i < 945; i++)
    {
        advance_motor(&(SControl.MControl), UP);
        _delay_us(3000);
    }
    SControl.MControl.current_pos = 0;

    initInput();
    sei();


    while (true)
    {
        controller_thread(&SControl);
        input_thread(&SControl);
    }

/*
    while (true)
    {
        set_gauge_target(&SControl, (315<<4));
        while (SControl.MControl.current_pos != SControl.target_pos) controller_thread(&SControl);
        set_gauge_target(&SControl, (0));
        while (SControl.MControl.current_pos != SControl.target_pos) controller_thread(&SControl);
        platform_toggle_status_led();
    }
*/

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
