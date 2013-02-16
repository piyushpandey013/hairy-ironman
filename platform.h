
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

#include "slide.h" // for ticks_t

#if !defined F_CPU
#error "F_CPU must be defined for platform.h"
#endif

#define cpu_clocks_per_us F_CPU / 1000000
#define cpu_clocks_per_ms F_CPU / 1000

#if defined (__AVR_ATmega32U4__)
#include "platform/atmega32u4.h"
#elif defined (__AVR_ATmega324P__)
#include "platform/atmega324p.h"
#elif defined (__AVR_ATmega328__)
#include "platform/atmega328.h"
#else
#define STEPPER_TIMER_INTERRUPT_vect TIMER1_COMPA_vect
#endif

extern const uint8_t timer_prescale;

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
