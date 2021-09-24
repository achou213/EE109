/********************************************
 *
 *  Name: Andrew Chou
 *  Email: achou213@usc.edu
 *  Section: Wednesday lab 12:30 - 1:50
 *  Assignment: Project
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "lcd.h"
#include "encoder.h"
#include "ds18b20.h"
#include "thermostat.h"

volatile unsigned char count_h = 90; 
volatile unsigned char count_l = 85; 
volatile unsigned char changed = 0;  // Flag for state change 
volatile int button_low = 0; 
volatile int button_high = 0;	
extern int far_temp; 

int main(void) {
	int fact_l =0, fact_h =0; 
	
    // Initialize DDR and PORT registers and LCD
	lcd_init();
	DDRD |= ((1 << PD2) | (1 << PD3)); //Initialize LED
	PORTD &=~((1 << PD2) | (1 << PD3));
	
	
	DDRC &= ~((1 << PC1) | (1 << PC2)); //initialize buttons as input
	PORTC |= ((1 << PC1) | (1 << PC2)); //enable pullup resistors

	//intialize encoder 
	encoder_init(); //function from encoder.c
	
	//initialize ds18b20
	ds_init(); 
	
    // Write a spash screen to the LCD
	lcd_clearscreen();			//Program I created to clear the screen
    lcd_writecommand(2);		// Write splash screen
    lcd_stringout("Andrew Chou"); //Prints out my name
	lcd_moveto(1, 0); 			//moves to position 1,0 of the LCD
	lcd_stringout("Project-Thermo"); //prints out the string phrase "Lab 7"
    _delay_ms(1000);	 		//delay for 1 sec
	lcd_clearscreen();			//clears screen again
	
	//read from EEPROM memory
	unsigned char mem_count_l, mem_count_h; //assign new char
	mem_count_l = eeprom_read_byte((void *) (300)); //set char as value in address 300
	mem_count_h = eeprom_read_byte((void *) (301)); //set char as valuein address 301
	
	//check if EEPROM values are temperature values (checking for incorrect EEPROM values)
	//if either condition met, replace initial values of count_l and count_h with values stores in EEPROM
	if((mem_count_h <= mem_count_l)){ //in the chance that low tolerance is greater than high tolerance, set back to initial values 
		mem_count_l = count_l; 
		mem_count_h = count_h; 
	}
	if((mem_count_l >= 40) && (mem_count_l <= 100)){
		count_l = mem_count_l; 
	}
	if((mem_count_h >= 40) && (mem_count_h <= 100)){
		count_h = mem_count_h; 
	}
	
	// check if mem high is less than mem low
	
	//print initial values
	lcd_writecommand(2);
	lcd_moveto(0,0);
	char temp_count[6];
	snprintf(temp_count, 6, "Temp:"); 
	lcd_stringout(temp_count);

	lcd_writecommand(2); 
	lcd_moveto(1,0); 
	char counter1[9];
	snprintf(counter1, 9, "Low=%03d", count_l);
	lcd_stringout(counter1);
	
	lcd_writecommand(2); 
	lcd_moveto(1,8); 
	char counter2[9];
	snprintf(counter2, 9, "High=%03d", count_h);
	lcd_stringout(counter2);
	
	check_sensor(); //function from thermostat.c
	
	ds_convert(); 
		
    while (1) { // Loop forever
		//print temperature from sensor
		print_temp(); //function from thermostat.c
		
		//check which buttons are pressed 
		//if low button pressed
		if((PINC & (1 << PC1)) == 0){ 
			_delay_ms(100); //debounce
			button_low = 1; 
			button_high = 0;
			
			lcd_writecommand(2);
			lcd_moveto(1,3); 
			lcd_stringout("?");
			
			lcd_moveto(1,12); 
			lcd_stringout ("=");
		}
		
		//if high button pressed
		if((PINC & (1 << PC2)) == 0){ 
			
			_delay_ms(100); //debounce
			button_high = 1; 
			button_low = 0; 
			
			lcd_writecommand(2); 
			lcd_moveto(1,12); 
			lcd_stringout ("?");
			
			lcd_moveto(1,3); 
			lcd_stringout("=");
		}
		
		//if encoder changes count value 
	
		if (changed) { 
	    changed = 0;    

	    // Output changed count to LCD
			if(button_low == 1)
			{
				//store memory in EEPROM
				eeprom_update_byte((void *) 300, count_l); //store count_l into eeprom
				update_low(); 
			}
		
			if(button_high == 1)
			{
				//store memory in EEPROM
				eeprom_update_byte((void *) 301, count_h); //store count_h into eepromm
				update_high();
			}
			
		} 
		
		//turn on/off AC/heater LED
		fact_l = count_l*10; //these two variables are here to aid LED process
		fact_h = count_h*10; 
		
		if(far_temp < (fact_l)){ //if temp is less than low tolerance 
			PORTD |=  (1 << PD2); //turn on heater LED
			PORTD &= ~(1 << PD3); 
		}else if(far_temp > (fact_h)){ //if temp is higher than high tolerance
			PORTD |=  (1 << PD3); //turn on AC LED
			PORTD &= ~(1 << PD2);
		}else if(far_temp > fact_l && far_temp < fact_h){  
			PORTD &= ~(1 << PD2); //turn off heater LED
			PORTD &= ~(1 << PD3); 
		}
	}
}





