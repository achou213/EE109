/*
  encoder.c - Routines for usinng the encoder 
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "encoder.h"
#include "lcd.h"

volatile unsigned char a,b; 
volatile unsigned char new_state, old_state;
extern volatile unsigned char count_h; 
extern volatile unsigned char count_l; 
extern volatile unsigned char changed;  // Flag for state change 
extern volatile int button_low; 
extern volatile int button_high;

void encoder_init(void){
	DDRB &= ~((1 << PB3) | (1 << PB4)); //initialize encoder as input
    PORTB |= ((1 << PB3) | (1 << PB4));
	
	//Initializing the interrupts 
	PCICR |= (1 << PCIE0); 
	PCMSK0 |= ((1 << PCINT4) | (1 << PCINT3)); 
	
	//initialize interrupts
	sei();
	
	
	// Read the A and B inputs to determine the intial state
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.   
	
	char PIN = PINB; 
	a = PIN & (1<< PB3);
	b = PIN & (1<< PB4); 
    if (!b && !a)
	old_state = 0;
    else if (!b && a)
	old_state = 1;
    else if (b && !a)
	old_state = 2;
    else
	old_state = 3;

    new_state = old_state;
}

void update_low(void){
	lcd_writecommand(2); 
	lcd_moveto(1,0); 
	char counter1[9];
	snprintf(counter1, 9, "Low?%03d", count_l);
	lcd_stringout(counter1);
}

void update_high(void){
	lcd_writecommand(2); 
	lcd_moveto(1,8); 
	char counter2[9];
	snprintf(counter2, 9, "High?%03d", count_h);
	lcd_stringout(counter2);
}

//interrupt service routine
ISR(PCINT0_vect){ 
	// Read the input bits and determine A and B
	char PIN = PINB; 
	a = PIN & (1<< PB3);
	b = PIN & (1<< PB4);
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
            
			if (button_low == 1)
			{
				count_l++; 
			}else if(button_high == 1)
			{
				count_h++; 
			}
        }
        else if (b){ //when encoder rotated counterclockwise
            new_state = 2;
            if (button_low == 1)
			{
				count_l--; 
			}else if(button_high == 1)
			{
				count_h--; 
			}
        }
    }
    else if (old_state == 1){
        // Handle A and B inputs for state 1
        if (b){
            new_state = 3;
            if (button_low == 1)
			{
				count_l++; 
			}else if(button_high == 1)
			{
				count_h++; 
			}
        }
        else if (!a){
            new_state = 0;
            if (button_low == 1)
			{
				count_l--; 
			}else if(button_high == 1)
			{
				count_h--; 
			}
        }
    }
    else if (old_state == 2){
        // Handle A and B inputs for state 2
    if (a){
		new_state = 3;
        if (button_low == 1)
			{
				count_l--; 
			}else if(button_high == 1)
			{
				count_h--; 
			}
    }
     else if (!b){
        new_state = 0;
        if (button_low == 1)
			{
				count_l++; 
			}else if(button_high == 1)
			{
				count_h++; 
			}
        }
	}
    else{	
		// Handle A and B inputs for state 3
        
        if (!a){
            new_state = 2;
            if (button_low == 1)
			{
				count_l++; 
			}else if(button_high == 1)
			{
				count_h++; 
			}
        }else if (!b){
            new_state = 1;
            if (button_low == 1)
			{
				count_l--; 
			}else if(button_high == 1)
			{
				count_h--; 
			}
        }
		}

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
		if(count_l > 100) 
		{
			count_l = 100; 
		}
		if(count_h > 100)
		{
			count_h = 100; 
		}
		if(count_l < 40)
		{
			count_l = 40; 
		}
		if(count_h < 40)
		{
			count_h = 40; 
		}
		if(button_low == 1){
			if(count_l >= count_h)
			{
				count_l = count_h;  
			}
		}
		if(button_high == 1){
			if(count_h <= count_l)
			{
				count_h = count_l;   
			}
		}
		
		changed = 1;
	    old_state = new_state;
	}	
}

