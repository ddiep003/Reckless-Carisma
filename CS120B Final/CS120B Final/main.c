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

void DisplayAxis(uint16_t x, uint16_t y)
{
	char a[20], b[20];
	LCD_DisplayString(1, itoa(x,a,10));
	LCD_DisplayString(17, itoa(y,b,10));
}

void MotorControl()
{
	
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	TimerOn();
	TimerSet(1);
	uint16_t x,y;
	InitADC();
	LCD_init();
	signed short min = -100;
	signed short max = 100;
	
	while(1)
	{
		LCD_Cursor(1);
		x=readadc(0);      //READ ADC VALUE FROM PA.0
		y=readadc(1);      //READ ADC VALUE FROM PA.1
		x=x-512;
		y=y-512;
		DisplayAxis(x, y);
		if(y >= min && y <= max)
		{
			PORTB = 0x00;
		}
		else if (y < min)
		{
			PORTB = 0x01;
			PORTD = 0x00;
		}
		else if (y > max)
		{
			PORTB = 0x00;
			PORTD = 0x01;
		}
		else
		{
			PORTB = 0x00;
		}
		
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		
	}
}