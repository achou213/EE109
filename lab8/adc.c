#include <avr/io.h>

#include "adc.h"

#define MASKBITS 0b00001111

void adc_init(void)
{
  ADMUX |= (1 << REFS0);  // Initialize the ADC
  ADMUX &=~(1 << REFS1); 
    
  ADMUX |= (1 << ADLAR); //set ADLAR bit in ADMUX to 1 to get 8-bit conversion 
  
  ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); //set ADPS[2:0] bits in ADCSRA to 1
  
  ADCSRA |= (1 << ADEN); //Set ADEN bit in ADCSRA to enable ADC module
  
}

unsigned char adc_sample(unsigned char channel)
{
    // Set ADC input mux bits to 'channel' value
	
	ADMUX &= ~MASKBITS; //cleared lower bits [3:0]
	ADMUX |= (channel & MASKBITS);
	
    // Convert an analog input and return the 8-bit result
	ADCSRA |= (1 << ADSC); 
	
	while((ADCSRA & (1 << ADSC)) != 0)
	{
	}
	
	unsigned char result = ADCH;
	return result; 
}
