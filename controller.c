
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <avr/io.h>

#include "controller.h"
#include "platform.h"
#include "timer.h"

// Define the limits of the gauge we're controlling

#define MOTOR_SWEEP_DEG 270 // whole degrees only, please
angle_t motor_sweep_deg = MOTOR_SWEEP_DEG << 4;                 // shift to [1/10/4] format
steps_t motor_steps     = (MOTOR_SWEEP_DEG * 3); // full-step is 1/3 degree; multiply and shift to integer format

// accel_delay is the delay, in microseconds, between steps, with the values carefully tuned
//time_us_t accel_delay[] = { 3000, 1500, 1000, 800, 600 };
time_us_t accel_delay[] = { 15000, 11250, 8437, 6328, 4746, 3559, 2669, 2002, 1501, 1126 };
uint8_t num_accel_delay = 10;

// half-step bit values written to the motor control port
//  port   1 2 3 4
//
//  10 = [ 1 0 1 0 ]
//   8 = [ 1 0 0 0 ]
//   9 = [ 1 0 0 1 ]
//   1 = [ 0 0 0 1 ]
//   5 = [ 0 1 0 1 ]
//   4 = [ 0 1 0 0 ]
//   6 = [ 0 1 1 0 ]
//   2 = [ 0 0 1 0 ]
//unsigned int motorStateMap[] = { 10, 8, 9, 1, 5, 4, 6, 2 };
//unsigned int motorStates = 8;

uint_fast8_t motorStateMap[] = { 3, 6, 12, 9 };
uint_fast8_t motorStates = 4;

//The following is from the datasheet, and allows us to tie pins 2/3 together
//unsigned int motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
//unsigned int motorStates = 6;;

// Controller singletons manipulated to remember state
struct step_controller SControl;

// Declarations

// utility unit conversion function
steps_t angle_to_steps( angle_t target_angle );

void init_controller(struct step_controller* c)
{
    // initialize c
    c->state = STOPPED;
    c->velocity = 0;
    c->current_pos = 0;
    c->target_pos = 0;
    c->num_accel_steps = num_accel_delay;
    c->direction = UP;

    c->MControl.state_index = 0;
    c->MControl.motor_byte = &PORTD;

    init_stepper_timer();
    
    // for testing
    init_debug_leds();
}

/*
void print_step_controller( struct step_controller* c )
{
    char state = 'E'; // 'E' is for error.
    switch(c->state)
    {
        case STOPPED:      state = 'S'; break;
        case ACCELERATING: state = 'A'; break;
        case DECELERATING: state = 'D'; break;
        case MAX:          state = 'M'; break;
        default:           state = 'E'; break;
    }
    printf("%c %c @ %3d -> %3d, %d\n", (c->direction == UP ? 'U' : 'D'), state, c->current_pos, c->target_pos, c->velocity );
}
*/

void toggle_led(void)
{
    PORTC ^= (1<<PORTC7);
}

void advance_motor( struct motor_controller* m, enum move_direction d )
{
    m->state_index = (m->state_index + (d == UP ? 1 : -1 )) % motorStates;
    *(m->motor_byte) = motorStateMap[m->state_index];
    toggle_led();
}

// Eventually, this will grow into its own thread. Dream big.
void controller_thread(struct step_controller* c)
{
    if (c->needs_update == true) {
        int16_t delta = ( c->direction == UP ? c->target_pos - c->current_pos : c->current_pos - c->target_pos );

        switch (c->state) {
            case STOPPED:
                if ( c->target_pos != c->current_pos ) {
                    c->direction = ( c->target_pos > c->current_pos ? UP : DOWN );
                    c->state = ACCELERATING;
                } else {
                    stop_stepper_timer();
                }
            break;

            case ACCELERATING:
                if (delta <= (signed)c->velocity) {
                    // we're near or past the target, so we start slowing down
                    c->state = DECELERATING;
                    c->velocity -= 1;
                } else {
                    if (c->velocity == c->num_accel_steps-1)
                        c->state = MAX;
                    else
                        c->velocity += 1;
                }
            break;

            case DECELERATING:
                if (delta > (signed)c->velocity) {
                    // too far away, we need to speed up
                    c->state = ACCELERATING;
                    c->velocity += 1;
                } else {
                    c->velocity -= 1;
                    if (c->velocity == 0)
                        c->state = STOPPED;
                }
            break;

            case MAX:
                if (delta <= (signed)c->velocity) {
                    c->velocity -= 1;
                    c->state = DECELERATING;
                }
            break;

            default:
            break;
        } // end switch (c->state)
    } // end if (c->needs_update)
}

void set_stepper_target(struct step_controller* c, steps_t new_target_pos )
{
    if (new_target_pos == c->target_pos)
        return; // no action required
    if (new_target_pos >= motor_steps)
        new_target_pos = motor_steps; // saturate if given error input


    // we check if we're stopped because we "arrived" since the last time we were called, and not just
    //  stopped on our way back in another direction; this avoids trying to advance the motor too soon.
    if (c->state == STOPPED && c->current_pos == c->target_pos)
    {
        c->target_pos = new_target_pos;
        c->direction = ( c->current_pos > new_target_pos ? DOWN : UP );
        request_timer_interrupt(); // fire the interrupt, which sets the timer interval, and moves the motor its initial step
        start_stepper_timer();     // start the timer
        c->velocity += 1;
    } 
    else // nope, we're already in the process of servicing some other move, so we just set the destination and go
    {
        c->target_pos = new_target_pos;
    }

}

steps_t angle_to_steps( angle_t target_angle )
{
    //                             [1/10/4]   *            [0/16/0] = [1/26/4] /            [1/10/4]     = [1/30/0]
    int32_t workspace = (int32_t)target_angle * (int32_t)motor_steps           / (int32_t)motor_sweep_deg;

    // the result is normalized by the ratio target_angle/motor_sweep_deg, so the can drop the high bits
    //  (unless, of course, target_angle > motor_sweep_deg, which, I guess, caveat scriptor)
    return (steps_t)workspace;
}

void set_gauge_target( struct step_controller* c, angle_t target_angle )
{
    if (target_angle > motor_sweep_deg)
        target_angle = motor_sweep_deg;

    steps_t target_pos = angle_to_steps( target_angle );
    set_stepper_target( c, target_pos );
}
