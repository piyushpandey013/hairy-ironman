
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

extern unsigned int motor_steps;
extern unsigned int motor_sweep_deg;

extern unsigned int step_target_register;

enum control_state
{
    STOPPED,
    ACCELERATING,
    DECELERATING,
    MAX
};

enum move_direction
{
    UP = 1,
    DOWN = -1
};

extern unsigned int accel_delay[];

extern unsigned int motorStateMap[];
extern unsigned int motorStates;

//The following is from the datasheet, and allows us to tie pins 2/3 together
//unsigned int motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
//unsigned int motorStates = 6;;

struct motor_controller
{
    unsigned int state_index;
    unsigned int motor_byte;
    // we can add pin addresses or whatever here later
};


struct step_controller
{
    enum move_direction direction;
    enum control_state state;
    unsigned int current_pos;
    unsigned int target_pos;
    unsigned int current_delay;
    unsigned int num_accel_steps;
    unsigned int velocity;
};


// Declarations

uint32_t ms_to_ticks( unsigned int delay_ms );

void set_stepper_target(struct step_controller* c, unsigned int target_pos );

void step_timer_handle(struct step_controller* c, struct motor_controller* m);

void advance_motor( struct motor_controller* m, enum move_direction d );

void print_step_controller( struct step_controller* c );

#endif