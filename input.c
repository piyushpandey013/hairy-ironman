
#include <stdint.h>

#include <avr/io.h>

#include "input.h"
#include "controller.h"
#include "platform.h"

volatile adc_t adc_reading; // 10-bit ADC
volatile bool adc_changed;

adc_t input_max = 0x400;
adc_t input_min = 0x000;

void initInput()
{
    platform_init_adc();
}

angle_t reading_to_angle( adc_t reading )
{
    //                     [0/10/0]    *          [1/10/4]        /      [0/10/0]       ==> [1/20/4] / [0/10/0] ==> [1/20/4]
    int32_t result =  (int32_t)reading * (int32_t)motor_sweep_deg / (uint32_t)input_max;

    // result is normalized by the ratio of reading/input_max, so we can drop the high bits.
    return (angle_t)result;
}

void adc_interrupt_handler()
{
    adc_reading = ADCW;
    adc_changed = true;
}

// One day, this will grow up to be a thread.
void input_thread()
{
    /*
    while (true)
    {
    */
        if (adc_changed)
        {
            adc_t buffer = adc_reading;

            //set_gauge_target( &SControl, reading_to_angle( buffer ) );
            set_gauge_target( &SControl, (buffer >> 4) ); // ADC is 12-bit, so we limit it to 8 bits so it fits nicely
        }
    /*
    }
    */
}
