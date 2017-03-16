#include <avr/io.h>
#include "io.c"
#include "timer.h"
#include "usart_ATmega1284.h"
void InitADC(void)
{
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //ENABLE ADC, PRESCALER 128
}
uint16_t readadc(uint8_t ch)
{
	ch&=0b00000111;         //ANDing to limit input to 7
	ADMUX = (ADMUX & 0xf8)|ch;  //Clear last 3 bits of ADMUX, OR with ch
	ADCSRA|=(1<<ADSC);        //START CONVERSION
	while((ADCSRA)&(1<<ADSC));    //WAIT UNTIL CONVERSION IS COMPLETE
	return(ADC);        //RETURN ADC VALUE
}
void DisplayAxis(uint16_t x, uint16_t y)
{
	char a[20], b[20];
	LCD_DisplayString(1, itoa(x,a,10));
	LCD_DisplayString(17, itoa(y,b,10));
}
signed short min = -50;
signed short max = 50;
void MotorControl(int x, int y)
{
	unsigned char output = 0;
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
	PORTB = output;
	return output;
}
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
void SendData(char x)
{
	if(USART_IsSendReady(0))
	{
		SetBit(PORTD, 5, 1);
		USART_Send(x, 0);
	}
	else
	{
		SetBit(PORTD, 5, 0);
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	TimerOn();
	TimerSet(10);
	uint16_t x,y;
	InitADC();
	LCD_init();
	initUSART(0);
	USART_Flush(0);
	int x_;
	int y_;
	unsigned char out;
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
		//SendData(out);
		if(y_ >= min && y_ <= max && x_ >= min && x_ <= max)
		{
			PORTB = 0x00;
			SendData(0x00);
		}
		else if (y_ < min)
		{
			//0101 0101
			PORTB = 0x05;
			SendData(0x05);
		}
		else if (y_ > max)
		{
			//1010 1010
			PORTB = 0x0A;
			SendData(0x0A);
		}
		else if (x_ < min)
		{
			//0101 0110
			PORTB = 0x09;
			SendData(0x09);
		}
		else if (x_ > max)
		{
			//10 10 1001
			PORTB = 0x06;
			SendData(0x06);
		}
		else
		{
			//SendData(0xFF);
		}
		//SendData(0xFF);
		if(USART_HasTransmitted(0))
		{
			//PORTB = 0x01;
		}
		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;
		
	}
}