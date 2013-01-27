
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>

#include "timer.h"
#include "controller.h"
#include "platform.h"
#include "slide.h"

unsigned int timer_prescale = 8;

void initTimer()
{
    // set up the timer
    TCCR1A |= (1<<WGM12);   // set the timer to run in CTC mode
    TIMSK1 |= (1<<OCIE1A); // enable the OCR1A match interrupt

}

void start_stepper_timer(void)
{
    TCCR1B |= (1<<CS11); // set the prescaler to /8 and start the clock

    // note: this function is called every time we set a new destination for
    //  the stepper; that's okay! writing successive ones has no effect.
}

void stop_stepper_timer(void)
{
    TCCR1B &= ~( (1<<CS12) | (1<<CS11) | (1<<CS10) ); // clear the prescaler bits and stop the clock
    TCNT1 = 0x0000; // reset the counter to zero, so we start anew
}

void set_stepper_timer_timeout( ticks_t timeout )
{
    OCR1A = timeout;
}

ticks_t ms_to_ticks( time_us_t delay_ms )
{
    ticks_t ticks_per_ms = f_cpu / timer_prescale / 1000; // yay compile-time optimizers
    return delay_ms * ticks_per_ms;
}

void stepper_timer_handler()
{
    set_stepper_timer_timeout( ms_to_ticks( accel_delay[SControl.velocity] ) );
    SControl.needs_update = true;
    advance_motor(&MControl, SControl.direction);
    SControl.current_pos += (SControl.direction == UP ? 1 : -1);
}


