
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#include "controller.h"

int main()
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

    set_stepper_target( &SControl, 30 );
    print_step_controller(&SControl);
    int limit = 100;
    step_timer_handle(&SControl, &MControl);
    while (SControl.state != STOPPED && --limit)
    {
        printf("[ %2d_%2d ] ", MControl.motor_byte, MControl.state_index );
        print_step_controller(&SControl);
        step_timer_handle(&SControl, &MControl);
    }

    return 0;
}
