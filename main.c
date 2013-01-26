
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#include "controller.h"

int main()
{

    struct step_controller c;
    struct motor_controller m;
    // initialize c
    c.state = STOPPED;
    c.velocity = 0;
    c.current_pos = 0;
    c.target_pos = 0;
    c.num_accel_steps = 5;
    //initialize m
    m.state_index = 0;
    m.motor_byte = 0;

    set_stepper_target( &c, 30 );
    print_step_controller(&c);
    int limit = 100;
    step_timer_handle(&c, &m);
    while (c.state != STOPPED && --limit)
    {
        printf("[ %2d_%2d ] ", m.motor_byte, m.state_index );
        print_step_controller(&c);
        step_timer_handle(&c, &m);
    }

    return 0;
}
