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
void DisplayAxis(uint16_t x, uint16_t y)
{
	char a[20], b[20];
	LCD_DisplayString(1, itoa(x,a,10));
	LCD_DisplayString(17, itoa(y,b,10));
}
signed short min = -100;
signed short max = 100;
void MotorControl(int x, int y)
{
	if(y >= min && y <= max && x >= min && x <= max)
	{
		PORTB = 0x00;
	}
	else if (y < min)
	{
		PORTB = 0x55;
	}
	else if (y > max)
	{
		PORTB = 0xAA;
	}
	else if (x < min)
	{
		//01 01 10 10
		PORTB = 0x5A;
	}
	else if (x > max)
	{
		//10 10 01 01
		PORTB = 0xA5;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xBF; PORTB = 0x40;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	TimerOn();
	TimerSet(1);
	uint16_t x,y;
	InitADC();
	LCD_init();

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

	int x_;
	int y_;
	while(1)
	{
		LCD_Cursor(1);
		x=readadc(0);      //READ ADC VALUE FROM PA.0
		y=readadc(1);      //READ ADC VALUE FROM PA.1
		x=x-512;
		y=y-512;
		x_ = x;
		y_ = y;
		DisplayAxis(x, y);
		MotorControl(x_, y_);
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		
	}
}