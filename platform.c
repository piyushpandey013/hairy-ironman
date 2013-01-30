
#include <avr/io.h>

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
