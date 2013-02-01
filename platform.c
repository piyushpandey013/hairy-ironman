
#include <avr/io.h>

void init_debug_leds(void)
{
#ifdef _AVR_IOM32U4_H_
    DDRD |= (1<<DDD0) | (1<<DDD1) | (1<<DDD2) | (1<<DDD3); // set motor pins to output
    DDRC |= (1<<DDC7);
#endif
}

void usb_workaround(void)
{
#ifdef _AVR_IOM32U4_H_
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
#endif
}
