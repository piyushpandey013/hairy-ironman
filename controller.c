
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "controller.h"
#include "platform.h"

unsigned int timer_prescale = 8;

unsigned int motor_steps = 270 * 3;
unsigned int motor_sweep_deg = 270;

unsigned int step_target_register;

unsigned int accel_delay[] = { 3000, 1500, 1000, 800, 600 };

unsigned int motorStateMap[] = { 10, 8, 9, 1, 5, 4, 6, 2 };
unsigned int motorStates = 8;

//The following is from the datasheet, and allows us to tie pins 2/3 together
//unsigned int motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
//unsigned int motorStates = 6;;


void print_step_controller( struct step_controller* c )
{
    char state = 'E';
    switch(c->state)
    {
        case STOPPED: state = 'S'; break;
        case ACCELERATING: state = 'A'; break;
        case DECELERATING: state = 'D'; break;
        case MAX: state = 'M'; break;
        default: state = 'E'; break;
    }
    printf("%c %c @ %3d -> %3d, %d\n", (c->direction == UP ? 'U' : 'D'), state, c->current_pos, c->target_pos, c->velocity );
}

void advance_motor( struct motor_controller* m, enum move_direction d )
{
    m->state_index = (m->state_index + (d == UP ? 1 : -1 )) % motorStates;
    m->motor_byte = motorStateMap[m->state_index];
}

void step_timer_handle(struct step_controller* c, struct motor_controller* m)
{
    signed int delta = ( c->direction == UP ? c->target_pos - c->current_pos : c->current_pos - c->target_pos );

    switch (c->state)
    {
        case STOPPED:
            if ( c->target_pos != c->current_pos )
            {
                c->direction = ( c->target_pos > c->current_pos ? UP : DOWN );
                c->state = ACCELERATING;
            }
        break;

        case ACCELERATING:
            if (delta <= (signed)c->velocity)
            {
                // we're close to or past the target, so we start slowing down
                c->state = DECELERATING;
                c->velocity -= 1;
            }
            else
            {
                c->velocity += 1;
                if (c->velocity == c->num_accel_steps-1)
                    c->state = MAX;
            }
        break;

        case DECELERATING:
            if (delta > (signed)c->velocity)
            {
                // too far away, we need to speed up
                c->state = ACCELERATING;
                c->velocity += 1;
            }
            else
            {
                c->velocity -= 1;
                if (c->velocity == 0)
                    c->state = STOPPED;
            }
        break;

        case MAX:
            if (delta <= (signed)c->velocity)
            {
                c->state = DECELERATING;
                c->velocity -= 1;
            }
        break;

        default:
        break;
    } // end switch (c->state)

    if (c->state != STOPPED)
    {
        step_target_register = ms_to_ticks( accel_delay[ c->velocity ] );
        advance_motor(m, c->direction);
        c->current_pos += c->direction ? 1 : -1;
    }
}

void set_stepper_target(struct step_controller* c, unsigned int target_pos )
{
    if (target_pos == c->target_pos)
        return; // no action required
    if (target_pos >= motor_steps)
        target_pos = motor_steps; // saturate if given error input

    c->target_pos = target_pos;
}


uint32_t ms_to_ticks( unsigned int delay_ms )
{
    unsigned int ticks_per_ms = f_cpu / timer_prescale / 1000; // yay compile-time optimizers
    return delay_ms * ticks_per_ms;
}

