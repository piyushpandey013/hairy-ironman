
#include <stdint.h>

#include <avr/io.h>

#include "controller.h"
#include "platform.h"

// This file provides an interface for controlling the "slide" of the pins

typedef uint8_t pin_t;
typedef uint16_t ticks_t;

const uint8_t PWMDAC_MAX = 0xFF; // the value that causes the PWM or DAC to reach 100%
const uint8_t PWMDAC_MIN = 0x00; // the value that causes the PWM or DAC to reach 0%

const uint8_t PWMDAC_STEPS = 0x8; // the number of states between PWMDAC_MIN and PWMDAC_MAX

struct pwm_slider
{
    uint8_t pwm_subcycles_per_step;
    uint8_t curr_pwm_subcycle;
    uint8_t curr_slide_step;
    enum move_direction slide_direction;
    volatile uint8_t* counter_reg;
    volatile uint8_t* compare_reg;
};

struct pwm_slider pwm1_slider;
struct pwm_slider pwm2_slider;

void initSliders()
{
    pwm1_slider.slide_direction = DOWN;
    pwm1_slider.compare_reg = &OCR0A; // store the address of the compare register
    pwm1_slider.counter_reg = &TCNT0; // store the address of the counter register

    pwm2_slider.slide_direction = DOWN;
    pwm2_slider.compare_reg = &OCR0B; // store the address of the compare register
    pwm2_slider.counter_reg = &TCNT0; // store the address of the counter register

    //TODO set up timers here
    //set compare pins to output mode
    //set compare values so that the pins don't go high yet
    //start timers and enable their interrupts

    // initialize them such that they both remain unpowered
}

void slider_timer_handler()
{
    if (pwm1_slider.curr_pwm_subcycle != 0)
    {
        pwm1_slider.curr_pwm_subcycle -= 1;
        return;
    }

    if (   ( pwm1_slider.curr_slide_step == 0            && pwm1_slider.slide_direction == DOWN) 
        || ( pwm1_slider.curr_slide_step == PWMDAC_STEPS && pwm1_slider.slide_direction == UP  ) ) 
    {
        return;
    }

    pwm1_slider.curr_slide_step += (pwm1_slider.slide_direction == UP ? 1 : -1);
    pwm1_slider.curr_pwm_subcycle = pwm1_slider.pwm_subcycles_per_step;
    *(pwm1_slider.compare_reg) = PWMDAC_MAX * pwm1_slider.curr_slide_step / PWMDAC_STEPS; // set the compare register to the proportion needed
}

void startSlide( struct pwm_slider* slider, enum move_direction m, uint16_t duration_us )
{
    const uint32_t delay_us_per_step = duration_us / PWMDAC_STEPS; // each "step" of the pwm or dac will take this long, in microsec
    const uint32_t cpu_clocks_per_step = delay_us_per_step * cpu_clocks_per_us; // each "step" will take this long, in cpu cycles
    const uint32_t pwm_substeps_per_step = cpu_clocks_per_step / 256; // the pwm is running at CPU clock speed, so we'll have this many pwm overflows between level changes

    slider->pwm_subcycles_per_step = pwm_substeps_per_step;
    slider->curr_pwm_subcycle = pwm_substeps_per_step;
    slider->curr_slide_step = (m == UP ? 0 : PWMDAC_STEPS);
    slider->slide_direction = m;
    *(slider->compare_reg) = PWMDAC_MAX / PWMDAC_STEPS * slider->curr_slide_step;
    *(slider->counter_reg) = 0; // reset the counter
}

void setSliderFixed( struct pwm_slider* slider, enum move_direction m )
{
    slider->curr_slide_step = (m == UP ? PWMDAC_STEPS : 0);
    slider->slide_direction = m;
    *(slider->compare_reg) = (m == UP ? PWMDAC_MAX : PWMDAC_MIN);
}
