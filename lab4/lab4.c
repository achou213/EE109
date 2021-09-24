/********************************************
*
*  Name: Andrew Chou
*  Email: achou213@usc.edu
*  Section: Wednesday lab 12:30 - 1:50
*  Assignment: Lab 4 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

char checkInput(char);

int main(void) {

	DDRC  &=~(1 << 4); // Setup DDR and PORT bits for the 2 input buttons
	DDRC  &=~(1 << 2); // as necessary
	PORTC |= (1 << 4); //initialize button ports and pull-up resistors
	PORTC |= (1 << 2); 
 
    lcd_init(); // Initialize the LCD
        
					// Use a state machine approach to organize your code
					//   - Declare and initialize a variable to 
					//     track what state you are in
		
	int state = 0; //0 means counting up and 1 means counting down 
	int count = 0; //current count value
	int ct = 0; //counter value to check buttons more frequently

    while (1) { // Loop forever

                // Use an outer if statement to select what state you are in
                // Then inside each 'if' or 'else' block, perform the desired
                // output operations and then sample the inputs to update
                // the state for the next iteration
				
		lcd_writecommand(2); //Move to row 0, column 0
		_delay_ms(100); 		 // Delay before we go to the next iteration of the loop
								//counter speed
	
		if(checkInput(PC4) == 0) //if red button is pressed: 
			{
				state = 1; 			//counter moves down 
			}
		else if(checkInput(PC2) == 0) //if green button is pressed
			{
				state = 0; 			//counter moves up 
			}
							
		ct++;		 //ct increases by increment of 1
		if (ct == 5) //check if counter value for buttons is 5
		{
			ct = 0; //resets counter 
				
			if (state == 0){ //if the counter is counting up 
				count++; 		//counter increases by an increment of 1
					if(count > 9) //if counter value exceeds value of 9 
					{ 
						count = 0; //program prints out value 0 
					}
				lcd_writedata(count + '0'); //lcd prints out current counter value
				}	
					
			else if(state == 1){
				count--; 		//value counter decreases by increment of 1 (counts down by 1) 
					if(count < 0) //if counter value is less than 0 
					{	
						count = 9; //program prints out value 9 
					}
				lcd_writedata(count + '0'); //lcd prints out the current counter value
				}			
			}
	}
	return 0;   /* never reached */
}

char checkInput(char bit)
{	
	//function copied from last lab to check if button is pressed 
	//this condition checks if port input register and button is an input. 
	//If it is not, then it will be an output.  
if ((PINC & (1 << bit)) == 0)
        return (0); //output 0
    else
        return (1); //output 1
}
