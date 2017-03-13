/*
 * CS120BFinal2.c
 *
 * Created: 3/12/2017 11:00:15 PM
 * Author : Danyo3
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include "io.h"
#include "io.c"
#include "lcd.h"
#include "lcd.c"
#include <util/delay.h>
#include "timer.h"
#include "adc.c"
#include "nRF2401_Transmitter.c"

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xBF; PORTB = 0x40;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	clockprescale();
	usart_init();
	InitSPI();
	ioinit();
	INT0_interrupt_init();
	USART_interrupt_init();
	nrf24L01_init();

	SETBIT(PORTB,0);		// In order to ensure that the diode works!
	_delay_ms(1000);
	CLEARBIT(PORTB,0);
	unsigned char receiveData;
    while (1) 
    {
		receiveData = WriteByteSPI();
    }
}

