
#include <stdint.h>

#include <avr/io.h>

#include "input.h"
#include "controller.h"

volatile adc_t adc_reading; // 10-bit ADC
volatile bool adc_changed;

adc_t input_max = 0x400;
adc_t input_min = 0x000;

void initInput()
{
    // init ADC
    // free-running mode
    // some "reasonable" speed
    // default values to 0
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

void adc_thread()
{
    while (true)
    {
        if (adc_changed)
        {
            adc_t buffer = adc_reading;

            set_gauge_target( &SControl, reading_to_angle( buffer ) );
        }
    }
}
