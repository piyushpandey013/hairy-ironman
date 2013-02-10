
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

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
//uint8_t num_accel_delay = 5;
//time_us_t accel_delay[] = { 15000, 11250, 8437, 6328, 4746, 3559, 2669, 2002, 1501, 1126 };
//uint8_t num_accel_delay = 10;
time_us_t accel_delay[] = { 65000, 65000, 65000, 65000, 65000, 65000 };
uint8_t num_accel_delay = 6;

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

//The following is from the datasheet, and allows us to tie pins 2/3 together
uint_fast8_t motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
uint_fast8_t motorStates = 6;

// Controller singletons manipulated to remember state
struct step_controller SControl;

// Declarations

// utility unit conversion function
steps_t angle_to_steps( angle_t target_angle );

void init_controller(struct step_controller* c)
{
    // for testing
    init_debug_leds();

    // initialize c
    c->state = STOPPED;
    c->target_pos = 0;

    c->MControl.velocity = 0;
    c->MControl.direction = UP;
    c->MControl.current_pos = 0;
    c->MControl.state_index = 0;
    c->MControl.num_accel_steps = num_accel_delay;

    init_stepper_timer();
    start_stepper_timer();
    set_stepper_timer_timeout_us( accel_delay[0] );
    
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
    platform_toggle_status_led();
}

void advance_motor( struct motor_controller* m, enum move_direction d )
{
    m->state_index = (m->state_index + (d == UP ? 1 : motorStates-1 )) % motorStates;
    *(motor_port) = motorStateMap[m->state_index];
    /*
    uint8_t mask = motorStateMap[m->state_index];
    for (int i = 0; i < num_motor_pins; i++)
    {
        bool bit = mask & 0x1;
        if (bit) // set bit high
            *(motor_ports[i]) |= bit << motor_pins[i];
        else
            *(motor_ports[i]) &= ~(bit << motor_pins[i]);
        mask >>= 1;
    }
    */
    toggle_led();
}

// Eventually, this will grow into its own thread. Dream big.
void controller_thread(struct step_controller* c)
{
    if (c->needs_update == true) {
        c->needs_update = false;

        // the casts in the following line may cause truncation problems - do you need more than 2^15-1 positions?
        int16_t delta = (c->MControl.direction == UP ? (int16_t)c->target_pos - (int16_t)c->MControl.current_pos : (int16_t)c->MControl.current_pos - (int16_t)c->target_pos);

        if (delta == 0 && c->MControl.velocity == 0)
        {
            c->state = STOPPED;
            return;
        }

        if (delta < 0 && c->MControl.velocity == 0) // we're past our target and we've come to a stop
        {
            c->MControl.direction = ( c->MControl.direction == UP ? DOWN : UP ); // reverse direction
            c->state = ACCELERATING;

            return;
        }

        if (delta <= (signed)c->MControl.velocity)
            c->state = DECELERATING;
        else if (c->MControl.velocity == c->MControl.num_accel_steps -1)
            c->state = MAX;
        else
            c->state = ACCELERATING;

    }

}

void set_stepper_target(struct step_controller* c, steps_t new_target_pos )
{
    if (new_target_pos == c->target_pos)
        return; // no action required
    if (new_target_pos >= motor_steps)
        new_target_pos = motor_steps; // saturate if given error input

    c->target_pos = new_target_pos;
    c->needs_update = true;

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
