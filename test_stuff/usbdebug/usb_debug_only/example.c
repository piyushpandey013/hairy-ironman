#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usb_debug_only.h"
#include "print.h"
#include "analog.h"

#define LED_CONFIG	(DDRC |= (1<<DDC7))
#define LED_ON		(PORTC &= ~(1<<PORTC7))
#define LED_OFF		(PORTC |= (1<<PORTC7))
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

void usb_workaround(void);

int main(void)
{
	uint16_t val, count=1;

	DDRC |= (1<<DDC7);
	PORTC |= (1<<PORTC7);

usb_workaround();

	usb_init();
	while (!usb_configured()) /* wait */ ;
	PORTC ^= (1<<PORTC7);

	// wait an extra second for the PC's operating system
	// to load drivers and do whatever it does to actually
	// be ready for input
	_delay_ms(1000);
	PORTC ^= (1<<PORTC7);

	// start printing stuff.  If hid_listen is running on
	// the host, this should appear.
	print("USB debug only example\n");
	while (1) {
		print("Hello World ");
		phex16(count++);
		val = analogRead(0);
		print(", ADC0=");
		phex16(val);
		print("\n");
		_delay_ms(1000);
	}
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
