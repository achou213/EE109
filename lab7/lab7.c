/********************************************
 *
 *  Name: Andrew Chou
 *  Email: achou213@usc.edu
 *  Section: Wednesday lab 12:30 - 1:50
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>


#include "lcd.h"
#include <avr/interrupt.h>

void variable_delay_us(int);
void play_note(unsigned short);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
unsigned short frequency[8] =
    { 262, 294, 330, 349, 392, 440, 494, 523 };

//declare global variables 
//changed variable type to volatile because variables can change outside of main stream of code
volatile unsigned char new_state, old_state;
volatile unsigned char changed = 0;  // Flag for state change 					
volatile unsigned char a, b;
volatile int count = 0;		// Count to display

int main(void) {	
	
    // Initialize DDR and PORT registers and LCD
	lcd_init();
	DDRC &= ~((1 << PC1) | (1 << PC5)); //initialize encoder as input
    PORTC |= ((1 << PC1) | (1 << PC5));
    DDRB |= (1 << PC4);	//initiliaze the buzzer port as output 
    PORTB &= ~(1 << PC4); 
	
    // Write a spash screen to the LCD
	lcd_clearscreen();			//Program I created to clear the screen
    lcd_writecommand(2);		// Write splash screen
    lcd_stringout("Andrew Chou"); //Prints out my name
	lcd_moveto(1, 0); 			//moves to position 1,0 of the LCD
	lcd_stringout("Lab 7- Encoder");		//prints out the string phrase "Lab 7"
    _delay_ms(1000);	 		//delay for 1 sec
	lcd_clearscreen();			//clears screen again

	//display initial count value (should be zero)
	lcd_writecommand(2); 
	lcd_moveto(1,5); 
	char counter[5];
	snprintf(counter, 5, "%03d", count); //the %03 prints out 3 digits 
	lcd_stringout(counter);
	
    // Read the A and B inputs to determine the intial state
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.   
	
	a = PINC & (1<< PC1);
	b = PINC & (1<< PC5); 
    if (!b && !a)
	old_state = 0;
    else if (!b && a)
	old_state = 1;
    else if (b && !a)
	old_state = 2;
    else
	old_state = 3;

    new_state = old_state;
	
	//Initializing the interrupts 
	PCICR |= (1 << PCIE1); 
	PCMSK1 |= ((1 << PCINT9) | (1 << PCINT13)); 
	
    while (1) {                 // Loop forever
	//the first part of the code is moved to ISR(PCINT1_vect) below
	//to incorportate interrupts within our circuit
	
		sei(); //enable global variables/interrupts	
        if (changed) { // Did state change?
	    changed = 0;        // Reset changed flag

	    // Output changed count to LCD
		lcd_writecommand(2); 
		lcd_moveto(1,5); 
		char counter[5];
		snprintf(counter, 5, "%03d", count);
		lcd_stringout(counter);
	    // Do we play a note?
		if ((count % 8) == 0) { //using modulus checks if a number is divisible by the number after modulus
		
		// Determine which note (0-7) to play
		// Find the frequency of the note
		int note_num = (abs(count) % 64) / 8; //using equation from writeup to calculate which frequency to output
		play_note(frequency[note_num]); // Call play_note and pass it the frequency
										//using the frequency array above, play out the corresponding frequency
	    }
        }
    }
}

//interrupt service routine
ISR(PCINT1_vect){ 
	// Read the input bits and determine A and B
	a = PINC & (1<< PC1);
	b = PINC & (1<< PC5);
	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	
	//following the state diagram for rotary encoder
	if (old_state == 0)
		{
		//00 - state 0
		//01 - state 1
		//10 - state 2
		//11 - state 3
        // Handle A and B inputs for state 0
        if (a){ //when encoder rotated clockwise
            new_state = 1;
            count++;	//count number increases when encoder rotated is CW
        }
        else if (b){ //when encoder rotated counterclockwise
            new_state = 2;
            count--;	//count number decreases when encoder rotated CCW
        }
    }
    else if (old_state == 1){
        // Handle A and B inputs for state 1
        if (b){
            new_state = 3;
            count++;
        }
        else if (!a){
            new_state = 0;
            count--;
        }
    }
    else if (old_state == 2){
        // Handle A and B inputs for state 2
    if (a){
		new_state = 3;
        count--;
    }
     else if (!b){
        new_state = 0;
        count++;
        }
		}
    else{	
		// Handle A and B inputs for state 3
        
        if (!a){
            new_state = 2;
            count++;
        }else if (!b){
            new_state = 1;
            count--;
        }
		}

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
	    changed = 1;
	    old_state = new_state;
	}	
}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(unsigned short freq)
{
    unsigned long period;
	
    period = 1000000 / freq;      // Period of note in microseconds

    while (freq--) {
	// Make PB4 high
	PORTB |= (1 << PB4); 
	// Use variable_delay_us to delay for half the period
	variable_delay_us(period/2);
	// Make PB4 low
	PORTB &=~(1 << PB4);
	// Delay for half the period again
	variable_delay_us(period/2); 
    }
}

/*
    variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;

    while (i--)
        _delay_us(10);
}

