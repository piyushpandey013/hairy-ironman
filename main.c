
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#include "controller.h"
#include "input.h"

int main()
{

    initController();
    initInput();

    set_gauge_target( &SControl, (30<<4) );
    
    // start controller and ADC threads

    while (true)
    {};

    return 0;
}
