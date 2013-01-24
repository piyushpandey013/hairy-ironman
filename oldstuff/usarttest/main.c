
#include <avr/io.h>
#ifndef F_CPU
//define cpu clock speed if not defined
#define F_CPU 8000000
#endif
//set desired baud rate
#define BAUDRATE 115200
//calculate UBRR value
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)

void USART_Init()
{
  //Set baud rate
  UBRRL=UBRRVAL;	 //low byte
  UBRRH=(UBRRVAL>>8);	//high byte
  //Set data frame format: asynchronous mode,no parity, 1 stop bit, 8 bit size
  UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0);
  //Enable Transmitter and Receiver
  UCSRB=(1<<RXEN)|(1<<TXEN);
}

void USART_bounce()
{
  //defien temp value for storing received byte
  uint8_t Temp;
  //wait until data is received
  while(!(UCSRA&(1<<RXC))){};
  //store received data to temp
  Temp=UDR;
  //wait for empty transmit buffer
  while (!(UCSRA&(1<<UDRE))){};
  //send received data back
  UDR=Temp;
}

int main(void)
{
  USART_Init();
  while(1)
    USART_bounce();
  return 0;
}