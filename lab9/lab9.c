/********************************************
 *
 *  Name: Andrew Chou
 *  Email: achou213@usc.edu
 *  Section: Wednesday lab 12:30 - 1:50
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/
/*PLEASE READ: 
Due to a hardware issue, my tri-state buffer is broken and I cannot incorporate it in my circuit.
After speaking with professor Redekopp and professor Weber, they told me that the only solution to 
replace the tri-state buffer is to use a wire and connect the TX and RX port so that data directly transfers
to the two ports. I will demonstrate the lab a bit differently, however, my code should be the same as everyone
else's. The only difference is that I am not using a tri-state buffer. If there are any issues, please discuss with
the professors. 
*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "adc.h"
#include "check9.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register

// ADC functions and variables

#define ADC_CHAN 0              // Buttons use ADC channel 0
volatile unsigned char adc_result; 
volatile int count = 0; 
volatile int changed = 0; 
volatile int flag = 0; 
volatile char word[16]; 
volatile int buffer = 0; 

char *messages[] = {
    "Hello           ",
    "How are you?    ",
    "EE109 is fun!   ",
	"Good morning!   ",
	"Good night!     ",
	"Sophomore 2021  ",
	"Electrical Eng  ",
	"EE109 is hard!  ",
	"Coronavirus :(  ",
	"Zoom University "
};

int main(void) {
	
    // Initialize the LCD, ADC and serial modules
	lcd_init();
	adc_init();
	
    // Initialize the baud rate checker
    check9_init();
	serial_init(MYUBRR); 
	
	// Enable receiver interrupts
	UCSR0B |= (1 << RXCIE0);    
	
    // Enable interrupts
	sei();

    // Show the splash screen
	lcd_clearscreen();			//Program I created to clear the screen
    lcd_writecommand(2);		// Write splash screen
    lcd_stringout("Andrew Chou"); //Prints out my name
	lcd_moveto(1, 0); 			//moves to position 1,0 of the LCD
	lcd_stringout("Lab 9- Serial");		//prints out the string phrase "Lab 7"
    _delay_ms(1000);	 		//delay for 1 sec
	lcd_clearscreen();			//clears screen again
	
	lcd_clearscreen();			//Program I created to clear the screen
    lcd_writecommand(2);		// Write splash screen
	lcd_moveto(0, 0); 			//moves to position 1,0 of the LCD
	char message[16];
	snprintf(message, 16, "%s" , messages[count]);
	_delay_ms(1000); 
	lcd_stringout(message);

    while (1) {                 // Loop forever

	// Get an ADC sample to read buttons
	adc_result = adc_sample(0); 
	
        // Up button pressed?
		if(adc_result >= 45 && adc_result <= 51)
		{
			_delay_ms(100);
			if(count == 0)
			{
				count = 9; 
			}else
				count--;
			changed = 1; 
		}

        // Down button pressed?
		if(adc_result >= 99 && adc_result <= 101)
		{
			_delay_ms(100);
			
			if(count == 9)
			{
				count = 0; 
			}else
				count++;
			changed = 1; 
		}
		
		if(changed == 1)
		{
			lcd_writecommand(2);		// Write splash screen
			lcd_moveto(0, 0); 			//moves to position 1,0 of the LCD
			char message[17];
			snprintf(message, 17, "%s" , messages[count]);
			lcd_stringout(message); 
			changed = 0; 
		}
		
        // Select button pressed?
        if(adc_result >= 204 && adc_result <= 206)
		{
			serial_stringout(messages[count]);
		}

        // Message received from remote device?
		if(flag == 1) 
		{
			lcd_moveto(1,0);
			lcd_stringout(word); 
			flag = 0; 
			buffer = 0; 
		}
    }
}

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short ubrr_value)
{

    // Set up USART0 registers
	UBRR0 = ubrr_value;             // Set baud rate
	UCSR0C = (3 << UCSZ00);               // Async., no parity,
                                          // 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
	
    // Enable tri-state buffer
	DDRD |= (1 << 3); 
	PORTD &=~(1 << 3); //tri-state buffer enabler connected to PD3
}

void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = ch;
}

void serial_stringout(char *s)
{

    // Call serial_txchar in loop to send a string
    // (Very similar to lcd_stringout in lcd.c)
	int i = 0; 
	for(i = 0; i < 17; i++)
	{
	serial_txchar(s[i]); 
	}

}

ISR(USART_RX_vect)
{
	
    // Handle received character
	char ch; 

	ch = UDR0; 
	
	// Store in buffer
	word[buffer] = ch; 
	buffer++; 
    // If message complete, set flag
	if(buffer == 16)
	{
		flag = 1; 
	}
}
