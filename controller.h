
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef  int16_t angle_t; // Fixed-point 1/10/4 angle measurement, good for values of [-1024,1024]
typedef uint16_t steps_t; // Integer number of steps

extern steps_t motor_steps;
extern angle_t motor_sweep_deg;

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

//extern unsigned int motorStateMap[];
//extern unsigned int motorStates;
extern uint_fast8_t motorStateMap[];
extern uint_fast8_t motorStates;

//The following is from the datasheet, and allows us to tie pins 2/3 together
//unsigned int motorStateMap[] = {0x9, 0x1, 0x7, 0x6, 0xE, 0x8};
//unsigned int motorStates = 6;;

struct motor_controller
{
    unsigned int state_index;
    //unsigned int motor_byte;
    volatile uint8_t* motor_byte;
    // we can add pin addresses or whatever here later
};


struct step_controller
{
    enum move_direction direction;
    enum control_state state;
    steps_t current_pos;
    steps_t target_pos;
    //unsigned int current_delay;
    unsigned int num_accel_steps;
    unsigned int velocity;
    bool needs_update;
    struct motor_controller MControl;
};


// Declarations

extern struct step_controller  SControl;
//extern struct motor_controller MControl;

void init_controller(struct step_controller* c);

void advance_motor( struct motor_controller* m, enum move_direction d );

// void print_step_controller( struct step_controller* c );

void set_gauge_target( struct step_controller* c, angle_t target_angle );

void controller_thread(struct step_controller* c);
#endif
