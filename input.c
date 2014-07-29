
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

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

ISR(ADC_vect)
{
    adc_t new_reading = platform_read_adc();
    if (new_reading != adc_reading)
    {
        adc_changed = true;
        adc_reading = new_reading;
    }
    platform_toggle_status_led();
}

void input_thread()
{
        if (adc_changed)
        {
            adc_t buffer = adc_reading;

            set_gauge_target( &SControl, buffer<<2 );
        }
}
