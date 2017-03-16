#include <avr/io.h>
#include "io.c"
#include "timer.h"
#include "usart_ATmega1284.h"

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	TimerOn();
	TimerSet(100);
	initUSART(0);
	initUSART(1);
	USART_Flush(0);
	while(1)
	{
		if (USART_HasReceived(0))
		{
			PORTC = USART_Receive(0);
		}
		else
		{
			
		}
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		
	}
}