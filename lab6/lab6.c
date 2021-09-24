/********************************************
 *
 *  Name: Andrew Chou
 *  Email: achou213@usc.edu
 *  Section: Wednesday lab 12:30 - 1:50
 *  Assignment: Lab 6 - Analog-to-digital conversion (redo)
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"
#include "adc.h"

void rand_init(void);

int main(void)
{	
		unsigned char adc_result; 
		int pos_X = 0; 
		int new_x_pos2 = 0;
		int new_x_pos1 = 0; 
		int pos_C = 0; 
		int newpos_C = 0; 
		int count = 0; 
		
    // Initialize the LCD
	lcd_init(); 

    // Initialize the ADC
	adc_init(); 

    // Initialize the random number function
	rand_init(); 
	

    // Write splash screen
	//display name and lab number
	lcd_clearscreen(); 
	lcd_writecommand(2); 
	lcd_moveto(0,0);
	//call lcd_stringout to print your name	
	lcd_stringout("Andrew Chou");
	/* Use lcd_moveto to start at an appropriate column 
	in the bottom row to appear centered */
	lcd_moveto(1,0); 
	lcd_stringout("Lab 6 - Game"); 
	/* Delay 1 second */
	_delay_ms(1000); 
	//clear screen 
	lcd_clearscreen(); 

    // Find a random initial position of the 'X'
	pos_X = rand() % 16; 

    // Display the 'X' on the screen
	lcd_writecommand(2); 
	lcd_moveto(0,pos_X); 
	lcd_stringout("X"); 
	
	//Display the '^' on the screen
	lcd_writecommand(2); 
	pos_C = adc_sample(3) % 16;
	lcd_moveto(1,pos_C); 
	lcd_stringout("^"); 
	
    while (1) {                 // Loop forever
	// Check for button pressed
		/*	testing button values	
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

        // If pressed, move 'X' to new position
		// Check for button pressed
		
		new_x_pos2 = pos_X +1; //initializes variables that will be used to move "X"
		new_x_pos1= pos_X -1; 
		
		adc_result = adc_sample(0); //set unsigned char adc_result to the adc_sample channel 0
	
		if(adc_result >= 154 && adc_result <=156) // If left button pressed, move 'X' to new position
		{
			_delay_ms(100); //debounce
			if(pos_X != new_x_pos1) //tests if the new location of X is different than current position
				{
					lcd_moveto(0, pos_X); //if true removes the current print on the current position and 
					lcd_stringout(" ");	 	   //and prints out a new "X" in the new position
					
					lcd_moveto(0, new_x_pos1);
					lcd_stringout("X");

					pos_X = new_x_pos1; //stores new position value to current position variable
				}
				
		} 
		
		if(adc_result == 0)// If right button pressed, move 'X' to new position
		{
			_delay_ms(100); //debounce
			if (pos_X != new_x_pos2) 	//tests if the new location of X is different than current position
				{
					lcd_moveto(0, pos_X); //if true removes the current print on the current position and 
					lcd_stringout(" ");		   //and prints out a new "X" in the new position
							
					lcd_moveto(0, new_x_pos2);
					lcd_stringout("X");

					pos_X = new_x_pos2; //stores new position value to current position variable
				}
		}

		 // Do a conversion of potentiometer input
		 newpos_C = adc_sample(3) / 16; 

		// Move '^' to new position
		
		if (pos_C != newpos_C)
		{	
			lcd_moveto(1,pos_C);
			lcd_stringout(" ");
			
			lcd_moveto(1, newpos_C);
			lcd_stringout("^");
			
			pos_C = newpos_C;
		}
		

		// Delay
		_delay_ms(10); 

		// Check if '^' is aligned with 'X'
		if(pos_C == pos_X)
		{
			count++;
			if(count == 200)
			{
				lcd_clearscreen();
				lcd_writecommand(2);
				lcd_moveto(0,4);
				lcd_stringout("Congrats");
				lcd_moveto(1,4);
				lcd_stringout("you won!");
			}
		}else 
				count = 0;

    }

    return 0;   /* never reached */
}

void rand_init()
{
    int seed = 0;
    unsigned char i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++) {
	for (j = 1; j < 6; j++) {
	    x = adc_sample(j);
	    x &= 1;	// Get the LSB of the result
	    seed = (seed << 1) + x; // Build up the 15-bit result
	}
    }
    srand(seed);	// Seed the rand function
}
