/********************************************
*
*  Name: Andrew Chou
*  Email: achou213@usc.edu
*  Lab section: Wednesday 12:30 - 1:50 
*  Assignment: Lab 3
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);

int main(void)
{
    // Initialize appropriate DDR registers
	
    // Initialize the LED output to 0
	//giving bit 2 a value of 1 sets it as output
	DDRD |= (1 << 2); 
	//initiliazes so that when button is not pressed, LED is turned off
	PORTD &= ~(1 << 2); 
	//Set Port B bit 3,4,5,as inputs
	DDRB &= ~(1 << 3);
	DDRB &= ~(1 << 4);
	DDRB &= ~(1 << 5);
    // Enable the pull-up resistors for the 3 button inputs
	PORTB |= (1 << 3);
	PORTB |= (1 << 4);
	PORTB |= (1 << 5);
	
    // Loop forever
    while (1) {                 
    
	//  Use if statements and the checkInput()
	//  function to determine if a button
	//  is being pressed and then output
	//  the correct dot/dash sequence by
	//  calling the dot(), dash(), and 
	//  using appropriate delay functions
	
		//When yellow button pressed, LED will flash C-pattern morse code
		if (checkInput(PB3) == 0)
        {
			dash();
			dot();
			dash();
			dot();
        }
		//When blue button pressed, LED will flash S-pattern morse code
        else if (checkInput(PB4) == 0)
        {
            dot();
            dot();
            dot();
        }
		//When green button pressed, LED will flash U-pattern morse code
        else if (checkInput(PB5) == 0)
        {
            dot();
            dot();
            dash();
        }
		//time delay until the next button is pressed. 
		//The time length is 750 ms, which is 3 times the dot length interval. 
		//this process occurs after each button is pressed since it is in the while 
		//loop
        _delay_ms(3 * DOT_LENGTH);
    }

    return 0;   /* never reached */
}

/*
  dot() - Makes the output LED blink a "dot".

  Write code to generate a dot by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off
*/

void dot()
{
	// makes output 1 to turn on LED on for 250 msecs  (dot length interval)
    makeOutput(1);
    _delay_ms(DOT_LENGTH);

    //makes output 0 to turn off LED off for 250 msecs (dot length interval)
    makeOutput(0);
	_delay_ms(DOT_LENGTH);	
    
}

/*
  dash() - Makes the output LED blink a "dash".

  Write code to generate a dash by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay
  appropriate time after you turn the output off
*/

void dash()
{
	// makes output 1 to turn on the LED on for 750 msecs (3 times dot length interval)
    makeOutput(1);
    _delay_ms(3 * DOT_LENGTH);

    //makes out put 0 to Turn off the LED off for 250 msecs
    makeOutput(0);
    _delay_ms(DOT_LENGTH);
}

/*
  makeOutput() - Changes the output bit to either a zero 
  or one, based on the input argument "value".
  
  If the argument is zero, turn the output OFF,
  otherwise turn the output ON. 
  
  Do not use any delays here.  Just use bit-wise operations
  to make the appropriate PORT bit turn on or off.
*/

void makeOutput(char value)
{
	//depending on what value we specify in makeoutput, a code will operate. 
	//If value is 1, then LED will turn on. If value is 0, then LED turns off
if (value)
    {
        //sets bit 2 to 1 --> turns output on
        PORTD |= (1 << 2);
    }
    else
    {
        //clears bit 2 to 0 --> turns output off
        PORTD &= ~(1 << 2);
    }
}

/*
  checkInput(bit) - Checks the state of the input bit specified by the
  "bit" argument (0-7), and returns either 0 or 1 depending on its state.
  
  Write code to use the appropriate group's PIN register and determine if  
  the specified bit (which is passed as the argument) of that group is
  pressed or not.  Think carefully about what bit value means "pressed"
  when connected to a pushbutton.
 */
 
char checkInput(char bit)
{
	//this condition checks if port input register and button is an input. 
	//If it is not, then it will be an output.  
if ((PINB & (1 << bit)) == 0)
        return (0); //output 0
    else
        return (1); //output 1
}
