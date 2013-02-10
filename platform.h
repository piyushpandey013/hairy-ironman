
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

#include "slide.h" // for ticks_t

/*
static const uint32_t f_cpu = 20000000;

static const uint32_t cpu_clocks_per_us = f_cpu / 1000000;
static const uint32_t cpu_clocks_per_ms = f_cpu / 1000;
*/

// C is annoying sometimes
#define f_cpu 20000000L
#define cpu_clocks_per_us f_cpu / 1000000
#define cpu_clocks_per_ms f_cpu / 1000

#if defined (__AVR_ATmega32U4__)
#include "platform/atmega32u4.h"
#elif defined (__AVR_ATmega324P__)
#include "platform/atmega324p.h"
#elif defined (__AVR_ATmega328__)
#include "platform/atmega328.h"
#else
#define STEPPER_TIMER_INTERRUPT_vect TIMER1_COMPA_vect
#endif

/*
extern          uint8_t  num_motor_pins;
extern volatile uint8_t* motor_ports[];
extern          uint8_t  motor_pins[];
*/
extern uint8_t* motor_port;

void init_debug_leds(void);

void usb_workaround(void);

void platform_init_stepper_timer(void);

void platform_start_stepper_timer(void);

void platform_stop_stepper_timer(void);

void platform_set_stepper_timer_timeout( ticks_t timeout );

void platform_request_stepper_timer_interrupt(void);

void platform_zero_stepper_timer(void);

void platform_toggle_status_led(void);

void platform_light_status_led(void);

void platform_darken_status_led(void);

#endif
