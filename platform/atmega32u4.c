
#include <avr/io.h>

#include "../slide.h" // for ticks_t

//#include "../platform.h"

const uint8_t timer_prescale = 8;

volatile uint8_t* motor_port = &PORTD;

void platform_toggle_status_led(void)
{
    PORTC ^= (1<<PORTC7);
}

void platform_light_status_led(void)
{
        PORTC |= (1<<PORTC7);
}

void platform_darken_status_led(void)
{
        PORTC &= ~(1<<PORTC7);
}

void platform_request_timer_interrupt(void)
{
    //TCCR1C |= (1<<FOC1A); // strobe the interrupt bit to trigger an interrupt
    // that doesn't work in CTC mode
}

void platform_stop_stepper_timer(void)
{
    TCCR1B &= ~( (1<<CS12) | (1<<CS11) | (1<<CS10) ); // clear the prescaler bits and stop the clock
}

void platform_set_stepper_timer_timeout( ticks_t timeout )
{
    OCR1A = timeout;
}

void platform_zero_stepper_timer(void)
{
    TCNT1 = 0x0000;
}

void platform_start_stepper_timer(void)
{
    TCCR1B |= (1<<CS11);   // set the prescaler to /8 and start the clock
    //TCCR1B |= (1<<CS11) | (1<<CS10); // prescaler to /64 for testing
    TIMSK1 |= (1<<OCIE1A); // enable the input capture interrupt
}

void platform_init_stepper_timer(void)
{
    TCCR1B |= (1<<WGM12);  // set the timer to run in CTC mode
    OCR1A = 0xFFFF;
}

void init_debug_leds(void)
{
    DDRD |= (1<<DDD0) | (1<<DDD1) | (1<<DDD2) | (1<<DDD3); // set motor pins to output
    DDRC |= (1<<DDC7);
}

void usb_workaround(void)
{
   // Datasheet says that to power off the USB interface we have to do 'some' of: 
   //       Detach USB interface 
   //      Disable USB interface 
   //      Disable PLL 
   //      Disable USB pad regulator 

   // Disable the USB interface 
   USBCON &= ~(1 << USBE);
   // Disable the VBUS transition enable bit 
   USBCON &= ~(1 << VBUSTE);
   // Disable the VUSB pad 
   USBCON &= ~(1 << OTGPADE);
   // Freeze the USB clock 
   USBCON &= ~(1 << FRZCLK);
   // Disable USB pad regulator 
   UHWCON &= ~(1 << UVREGE);
   // Clear the IVBUS Transition Interrupt flag 
   USBINT &= ~(1 << VBUSTI);
   // Physically detact USB (by disconnecting internal pull-ups on D+ and D-) 
   UDCON |= (1 << DETACH);
}
