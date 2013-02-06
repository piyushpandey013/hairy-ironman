
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
    // set up the timer
    TCCR1B |= (1<<WGM12);  // set the timer to run in CTC mode

}

void start_stepper_timer(void)
{
//    TCCR1B |= (1<<CS11);   // set the prescaler to /8 and start the clock
    TCCR1B |= (1<<CS11) | (1<<CS10); // prescaler to /64 for testing
    TIMSK1 |= (1<<OCIE1A); // enable the input capture interrupt 

    // note: this function is called every time we set a new destination for
    //  the stepper; that's okay! writing successive ones has no effect.
}

void zero_stepper_timer(void)
{
    TCNT1 = 0x0000;
}

void stop_stepper_timer(void)
{
    TCCR1B &= ~( (1<<CS12) | (1<<CS11) | (1<<CS10) ); // clear the prescaler bits and stop the clock
    zero_stepper_timer();
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

ISR(TIMER1_COMPA_vect)
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
            set_stepper_timer_timeout( ms_to_ticks( accel_delay[SControl.MControl.velocity] ) );
            break;
        case DECELERATING:
            SControl.MControl.velocity -= 1;
            set_stepper_timer_timeout( ms_to_ticks( accel_delay[SControl.MControl.velocity] ) );
            break;
        default: // this catches state == MAX
            break;
    }
    advance_motor(&SControl.MControl, SControl.MControl.direction);
    SControl.MControl.current_pos += (SControl.MControl.direction == UP ? 1 : -1);
}

void request_timer_interrupt(void)
{
    //TCCR1C |= (1<<FOC1A); // strobe the interrupt bit to trigger an interrupt
    TIMER1_COMPA_vect();
}


