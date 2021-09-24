/********************************************
 *
 *  Name: Andrew Chou
 *  Email: achou213@usc.edu
 *  Section: Wednesday lab 12:30 - 1:50
 *  Assignment: Lab 8 - Digital Stopwatch
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"
#include "adc.h"
#include <avr/interrupt.h>

volatile unsigned tenths, ones, tens; 
volatile unsigned char adc_result; 
volatile int old_state = 0; //current state of the timer	
						   // = 0 - stopped
						   // = 1 - started 
						   // = 2 - lapped 
volatile int changed = 0; 
volatile int new_state = 0; 
 
int main(void)
{
	
	//initialize the LCD
	lcd_init(); 
	//initialize the ADC
	adc_init(); 
	
	
	//Introduction splash screen
	lcd_clearscreen();			//Program I created to clear the screen
    lcd_writecommand(2);		// Write splash screen
    lcd_stringout("Andrew Chou"); //Prints out my name
	lcd_moveto(1, 0); 			//moves to position 1,0 of the LCD
	lcd_stringout("Lab 8- Stopwatch");		//prints out the string phrase "Lab 7"
    _delay_ms(1000);	 		//delay for 1 sec
	lcd_clearscreen();			//clears screen again
	
	//Initialize counter/timer
	TCCR1B |= (1 << WGM12); //set to CTC mode
	TCCR1B &= ~(1 << WGM13);
	TIMSK1 |= (1 << OCIE1A); //Enable timer interrupt 
	OCR1A = 6250;
	TCCR1B |= ((1 << CS12)); 
	
	
    while (1) // Loop forever
	{     
		adc_result = adc_sample(0);
		/*		
		adc_result = adc_sample(0);
		char buf[5]; 
		snprintf(buf, 5, "%4d", adc_result);
		lcd_moveto(0,0); 
		lcd_stringout(buf); 
		
		Right - 0 
		Left - 155 +- 1 (154/156)
		Up - 51 
		Down - 101
		*/
		sei(); 
		
		if(old_state == 0) //if stopped
		{	
			TCCR1B &= ~((1 << CS12));
			if(adc_result == 51)
			{
				_delay_ms(100);
				new_state = 1; 
			}else if(adc_result == 101)
				{
				_delay_ms(100);
				tenths = 0; 
				ones = 0; 
				tens = 0; 
				TCCR1B &= ~((1 << CS12));
			}
		}
			
		if(old_state == 1) //if started
		{
			TCCR1B |= ((1 << CS12));
			if(adc_result == 51)
			{
				_delay_ms(100);
				new_state = 0; 
			}
				
			if(adc_result == 101)
				{
				_delay_ms(100); 
				new_state = 2; 	
			}
		}

		if(old_state == 2) //if lapped 
		{
			_delay_ms(100);  
			changed = 1; 
			if(adc_result == 101)
			{
				_delay_ms(100);
				new_state = 1; 
				changed = 0; 
			}
		}
		
			
		if (new_state != old_state) {
			old_state = new_state;
		}
		
		if (changed == 0){
		lcd_writecommand(2);
		lcd_moveto(0,2);
		lcd_writedata(tens + '0');
		lcd_writedata(ones + '0');
		lcd_stringout(".");
		lcd_writedata(tenths + '0');
		}
		
	}
	return 0;   /* never reached */

}

 ISR(TIMER1_COMPA_vect){
	tenths++;
	if(tenths > 9)
	{
		ones++;
		tenths =0; 
		if(ones > 9)
		{
			tens++;
			ones = 0; 
		}
	}
	if(tens == 5 && ones == 9 && tenths == 9)
	{
		tens = 0; 
		ones = 0; 
		tenths = 0; 
	}	
 }