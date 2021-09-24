/********************************************
*
*  Name: Andrew Chou
*  Email: achou213@usc.edu
*  Section: Wednesday lab 12:30 - 1:50
*  Assignment: Lab 5 - Combination Lock
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
int  curState(int state); 

int main(void) {
		unsigned char A, B; 
        // Setup DDR and PORT bits for the 2 input buttons
        // as necessary
		DDRC &= ~((1 << 2) | (1 << 4)); //set buttons as inputs
		PORTC |= ((1 << 2) | (1 << 4)); //enable pull-up resistors
		
        // Initialize the LCD
        lcd_init(); 
        
		//display name and birthday
		lcd_clearscreen(); 
		lcd_writecommand(2); 
		lcd_moveto(0,0);
		//call lcd_stringout to print your name
		lcd_stringout("Andrew Chou");
		/* Use snprintf to create a string with your birthdate */
		char birthday[10];
		snprintf(birthday,10, "%d-%d-%d", 03,13,2001);
		/* Use lcd_moveto to start at an appropriate column 
		in the bottom row to appear centered */
		lcd_moveto(1,5); 
		/* Use lcd_stringout to print the birthdate string */
		lcd_stringout(birthday); 
		/* Delay 1 second */
		_delay_ms(1000); 
		//clear screen 
		lcd_clearscreen(); 
        
		A = PINC & (1 << 4); //a button 
		B = PINC & (1 << 2); //b button 
		
		int old_state = 0; 
		int new_state = 0; 
		char *lock = "locked"; 
		int change = 0; 
		
		lcd_writecommand(2); 
		lcd_moveto(0,0); 
		char currentstate[15]; 
		snprintf(currentstate, 15, "State = S%d" , old_state); 
		lcd_stringout(currentstate); 
		lcd_moveto(1,0); 
		char checklocked[30]; 
		snprintf(checklocked, 30, "Status: %s", lock);
		lcd_stringout(checklocked);		
		
    while (1) {               // Loop forever
		A = PINC & (1 << 4); //a button 
		B = PINC & (1 << 2); //b button
		
		if(old_state == 0)
		{
			if(B == 0)
			{
				_delay_ms(100); 
				new_state = 1; 
			}
		}else if(old_state == 1)
		{
			if(A == 0)
			{
				_delay_ms(100); 
				new_state = 2; 
			}
		}else if(old_state == 2)
		{
			if(B == 0)
			{
				_delay_ms(100); 
				new_state = 1; 
			}else if(A ==0)
			{
				_delay_ms(100); 
				new_state = 3; 
			}
		}else if(old_state == 3)
		{
			if(A == 0)
			{
				_delay_ms(100); 
				new_state = 0; 
			}else if(B ==0)
			{
				_delay_ms(100); 
				new_state = 4; 
				lock = "unlocked"; 
			}
		}else if(old_state == 4)
		{
			if(A == 0 || B == 0)
			{
				_delay_ms(100); 
				new_state = 0; 
				lock = "locked";
			}				
		}
	
		
		if(new_state != old_state)
		{
			change = 1; 
			old_state = new_state;
		}
		
			if(change == 1)
			{
				lcd_writecommand(2); 
				lcd_moveto(0,0); 
				char currentstate[15]; 
				snprintf(currentstate, 15, "State = S%d" , old_state); 
				lcd_stringout(currentstate); 
				
				lcd_moveto(1,0); 
				char checklocked[30]; 
				snprintf(checklocked, 30, "Status: %s  ", lock);
				lcd_stringout(checklocked); 
			}
    }
	
    return 0;   /* never reached */
}


