
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "controller.h"
#include "platform.h"
#include "timer.h"

// Define the limits of the gauge we're controlling

#define MOTOR_SWEEP_DEG 270 // whole degrees only, please
angle_t motor_sweep_deg = MOTOR_SWEEP_DEG << 4;                 // shift to [1/10/4] format
steps_t motor_steps     = (MOTOR_SWEEP_DEG * 3); // full-step is 1/3 degree; multiply and shift to integer format

// accel_delay is the delay, in microseconds, between steps, with the values carefully tuned
time_us_t accel_delay[] = { 3000, 1500, 1000, 800, 600 };

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
unsigned int motorStateMap[] = { 10, 8, 9, 1, 5, 4, 6, 2 };
unsigned int motorStates = 8;

//The following is from the datasheet, and allows us to tie pins 2/3 together
//unsigned int motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
//unsigned int motorStates = 6;;

// Controller singletons manipulated to remember state
struct step_controller SControl;
struct motor_controller MControl;

// Declarations

// utility unit conversion function
steps_t angle_to_steps( angle_t target_angle );

void initController(void)
{
    // initialize c
    SControl.state = STOPPED;
    SControl.velocity = 0;
    SControl.current_pos = 0;
    SControl.target_pos = 0;
    SControl.num_accel_steps = 5;
    //initialize m
    MControl.state_index = 0;
    MControl.motor_byte = 0;
}


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

void advance_motor( struct motor_controller* m, enum move_direction d )
{
    m->state_index = (m->state_index + (d == UP ? 1 : -1 )) % motorStates;
    m->motor_byte = motorStateMap[m->state_index];
}

void controller_thread()
{
    while (true)
    {
        if (SControl.needs_update == true)
        {
            signed int delta = ( SControl.direction == UP ? SControl.target_pos - SControl.current_pos : SControl.current_pos - SControl.target_pos );

            switch (SControl.state)
            {
                case STOPPED:
                    if ( SControl.target_pos != SControl.current_pos )
                    {
                        SControl.direction = ( SControl.target_pos > SControl.current_pos ? UP : DOWN );
                        SControl.state = ACCELERATING;
                    }
                break;

                case ACCELERATING:
                    if (delta <= (signed)SControl.velocity)
                    {
                        // we're near or past the target, so we start slowing down
                        SControl.state = DECELERATING;
                        SControl.velocity -= 1;
                    }
                    else
                    {
                        if (SControl.velocity == SControl.num_accel_steps-1)
                            SControl.state = MAX;
                        SControl.velocity += 1;
                    }
                break;

                case DECELERATING:
                    if (delta > (signed)SControl.velocity)
                    {
                        // too far away, we need to speed up
                        SControl.state = ACCELERATING;
                        SControl.velocity += 1;
                    }
                    else
                    {
                        SControl.velocity -= 1;
                        if (SControl.velocity == 0)
                            SControl.state = STOPPED;
                    }
                break;

                case MAX:
                    if (delta <= (signed)SControl.velocity)
                    {
                        SControl.velocity -= 1;
                        SControl.state = DECELERATING;
                    }
                break;

                default:
                break;
            } // end switch (SControl.state)
        } // end if (SControl.needs_update)
    } // end while (true)
}

void set_stepper_target(struct step_controller* c, steps_t target_pos )
{
    if (target_pos == c->target_pos)
        return; // no action required
    if (target_pos >= motor_steps)
        target_pos = motor_steps; // saturate if given error input

    c->target_pos = target_pos;
    start_stepper_timer();
}

steps_t angle_to_steps( angle_t target_angle )
{
    steps_t target_pos;
    if (target_angle > motor_sweep_deg)
        target_pos = motor_steps; // gracefully handle overflow

    //                             [1/10/4]   *            [0/16/0] = [1/26/4] /            [1/10/4]     = [1/30/0]
    int32_t workspace = (int32_t)target_angle * (int32_t)motor_steps           / (int32_t)motor_sweep_deg;

    // the result is normalized by the ratio target_angle/motor_sweep_deg, so the can drop the high bits
    return (steps_t)workspace;
}

void set_gauge_target( struct step_controller* c, angle_t target_angle )
{
    steps_t target_pos = angle_to_steps( target_angle );
    set_stepper_target( c, target_pos );
}
