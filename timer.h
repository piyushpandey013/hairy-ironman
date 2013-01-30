
#ifndef TIMER_H_
#define TIMER_H_

#include "controller.h"
#include "slide.h"

typedef uint16_t time_us_t;

void init_stepper_timer();

void start_stepper_timer(void);

void stop_stepper_timer(void);

void zero_stepper_timer(void);

void request_timer_interrupt(void);

void stepper_timer_handler();

void set_stepper_timer_timeout( ticks_t timeout );

#endif
