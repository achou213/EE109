/*
  thermostat.c - Routines for using the thermostat
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "ds18b20.h"

volatile int far_temp = 0; 

void check_sensor(void){
	if (ds_init() == 0) {    // check if sensor is responding
        lcd_writecommand(2); 
		lcd_moveto(0,0); 
		lcd_stringout("Sensor not responding"); 
    } 
}

void print_temp(void){
	unsigned char temp[2]; 
	int far_1 = 0, far_2 = 0, far_3 = 0, far_4 = 0;

	//print out temperature
		if(ds_temp(temp)){
			//convert to celsius
			int cel_1 = ((temp[1] << 8) + (temp[0])); 
			int cel_2 = ((cel_1)); //440 --> 27.5
			/*
			int cel_3 = cel_2 % 10; 
			int cel_4 = (cel_2 / 10) %100; 
			*/
			
			//convert to farenheit
			//far_1 = 90/5; //= 18
			far_1 = ((9*cel_2)/8); 
			far_2 = (far_1+320); //2750
			far_3 = (far_2) % 10; //5
			far_4 = (far_2/10) % 100;  //27
			far_temp = far_2; 
			
			//print out temp value
			lcd_writecommand(2);
			lcd_moveto(0,6);
			char temp1[10];
			snprintf(temp1, 10, "%d.%d", far_4,far_3); 
			lcd_stringout(temp1); 
			ds_convert(); 
		}	
}
