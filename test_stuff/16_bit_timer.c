
#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void usb_workaround(void);

/*
int main()
{
    cli();

    DDRC |= (1<<DDC7) | (1<<DDC6);

    TCCR1A = 0;
    TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);
    TCCR1C = 0;

    TCNT1 = 0;

    OCR1A = 15625;
    //OCR1AH = 0x3d;
    //OCR1AL = 0x09;
    TIMSK1 |= (1<<OCIE1A);

    //sei();


    for (;;)
    {
        if (TCNT1 >= OCR1A)
        {
            PORTC ^= (1<<PORTC6);
            TIFR1 = (1<<OCF1A);
        }
    }
}

ISR(TIMER1_COMPA_vect)
{
    PORTC ^= (1<<PORTC7);
}
*/
/*
int main (void) 
{ 
   DDRC |= (1 << PORTC7) | (1<<PORTC6); // Set LED as output 

   TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode 
   OCR1A   = 15624; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64 

   TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu/64 

   for (;;) 
   { 
      if (TIFR1 & (1 << OCF1A)) 
      { 
          PORTC ^= (1 << PORTC7); // Toggle the LED 
          PORTC ^= (1 << PORTC6); // Toggle the LED 

          TIFR1 = (1 << OCF1A); // clear the CTC flag (writing a logic one to the set flag clears it) 
      } 
   } 
}
*/

int main (void) 
{ 

    usb_workaround();
   DDRC |= (1 << PORTC7) | (1<<PORTC6); // Set LED as output 

   TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode 

   TIMSK1|= (1 << OCIE1A); // Enable CTC interrupt 

   sei(); //  Enable global interrupts 

   OCR1A   = 15624; // Set CTC compare value to 16 Hz at 16 MHz AVR clock, with a prescaler of 64 

   TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu/64 

   for (;;) 
   { 

   } 
} 

void usb_workaround(void)
{
    // Bootloader workaround testing. 
   // 
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

ISR(USB_GEN_vect){PORTC^=(1<<PORTC6);}
ISR(USB_COM_vect){PORTC^=(1<<PORTC7);}
ISR(WDT_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER1_CAPT_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER1_COMPA_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER1_COMPB_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER1_COMPC_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER1_OVF_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER0_COMPA_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER0_COMPB_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER0_OVF_vect){PORTC^=(1<<PORTC7);}
ISR(SPI_STC_vect){PORTC^=(1<<PORTC7);}
ISR(USART1_RX_vect){PORTC^=(1<<PORTC7);}
ISR(USART1_UDRE_vect){PORTC^=(1<<PORTC7);}
ISR(USART1_TX_vect){PORTC^=(1<<PORTC7);}
ISR(ANALOG_COMP_vect){PORTC^=(1<<PORTC7);}
ISR(ADC_vect){PORTC^=(1<<PORTC7);}
ISR(EE_READY_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER3_CAPT_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER3_COMPA_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER3_COMPB_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER3_COMPC_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER3_OVF_vect){PORTC^=(1<<PORTC7);}
ISR(TWI_vect){PORTC^=(1<<PORTC7);}
ISR(SPM_READY_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER4_COMPA_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER4_COMPB_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER4_COMPD_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER4_OVF_vect){PORTC^=(1<<PORTC7);}
ISR(TIMER4_FPF_vect){PORTC^=(1<<PORTC7);}

/*
ISR(TIMER1_COMPA_vect) 
{ 
   PORTC ^= (1 << PORTC7); // Toggle the LED 
}
*/
ISR(BADISR_vect)
{
    PORTC ^= (1<< PORTC6);
    _delay_ms(500);
}
