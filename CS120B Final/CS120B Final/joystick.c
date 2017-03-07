#ifndef F_CPU
#define F_CPU 16000000UL
#endif
 
#include <avr/io.h>

#include "lcd.h"        //include LCD Library
#include <util/delay.h>
 
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
 
//int main(void)
{
    char a[20], b[20], c[20];   
    uint16_t x,y,z;
    
 
    InitADC();         //INITIALIZE ADC
    lcd_init(LCD_DISP_ON);  //INITIALIZE LCD
    lcd_clrscr();      
 
    while(1)
    {
        lcd_home();         
 
        x=readadc(0);      //READ ADC VALUE FROM PA.0
        y=readadc(1);      //READ ADC VALUE FROM PA.1
        x=x-512;
        y=y-512;
 
        itoa(x,a,10);    
        itoa(y,b,10);
        
 
        lcd_puts("x=");     //DISPLAY THE RESULTS ON LCD
        lcd_gotoxy(2,0);
        lcd_puts(a);
 
        lcd_gotoxy(7,0);
        lcd_puts("y=");
        lcd_gotoxy(9,0);
        lcd_puts(b);
 
        
    }
}