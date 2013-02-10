
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "controller.h"
#include "platform.h"
#include "slide.h"

unsigned int timer_prescale = 8;

void init_stepper_timer()
{
    platform_init_stepper_timer();
}

void start_stepper_timer(void)
{
    // note: this function is called every time we set a new destination for
    //  the stepper; that's okay! writing successive ones has no effect.
    platform_start_stepper_timer();
}

void zero_stepper_timer(void)
{
    platform_zero_stepper_timer();
}

void stop_stepper_timer(void)
{
    platform_stop_stepper_timer();
    platform_zero_stepper_timer();
}

void set_stepper_timer_timeout( ticks_t timeout )
{
    platform_set_stepper_timer_timeout(timeout);
}

ticks_t us_to_ticks( time_us_t delay_us )
{
    ticks_t ticks_per_us = f_cpu / timer_prescale / 1000; // yay compile-time optimizers
    return delay_us * ticks_per_us;
}

void set_stepper_timer_timeout_us( time_us_t timeout )
{
    set_stepper_timer_timeout( us_to_ticks( timeout ) );
}

ISR(STEPPER_TIMER_INTERRUPT_vect)
{
    SControl.needs_update = true;
    switch (SControl.state)
    {
        case STOPPED:
            // what do we do if the state is stopped?
            return;
            break;
        case ACCELERATING:
            SControl.MControl.velocity += 1;
            set_stepper_timer_timeout( us_to_ticks( accel_delay[SControl.MControl.velocity] ) );
            break;
        case DECELERATING:
            SControl.MControl.velocity -= 1;
            set_stepper_timer_timeout( us_to_ticks( accel_delay[SControl.MControl.velocity] ) );
            break;
        default: // this catches state == MAX
            break;
    }
    advance_motor(&SControl.MControl, SControl.MControl.direction);
    SControl.MControl.current_pos += (SControl.MControl.direction == UP ? 1 : -1);
}

void request_timer_interrupt(void)
{

    STEPPER_TIMER_INTERRUPT_vect();
}


