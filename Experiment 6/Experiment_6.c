/*  Experiment 6 lab */
/*  Tasks 1-4  */
/*  Group Members: John Truong, Philip Aldridge  */

				/****  Header Files  ****/

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"
#include <stdio.h>

const char wave_step[] = { 0x01, 0x02, 0x04, 0x08 };
const char full_step[] = { 0x03, 0x06, 0x0C, 0x09 };
const char half_step[] = { 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x9 };

				/*****************  Prototype Functions  *****************/
void delayUs(int n);
						/****************		Main Function  *****************/

int main(void){
	int i = 0;
	/*****	Initialization	*****/
	SYSCTL->RCGCGPIO |= 0x02;
	SYSCTL->RCGCGPIO |= 0x20;
	
	GPIOB->DEN |= 0x0F; // sets pins PB3-PB0 as digital pins
	GPIOB->DIR |= 0x0F; // set pins PB3-PB0 as output
	
	GPIOF->DIR |= 0x0C; 	// sets pins PF3 and PF2 as outputs
	GPIOF->DEN |= 0x0C; 	// sets pins PF3 and PF2 as digital pins
	GPIOF->DATA |= 0x0C;	// asserts the pins into the data register
	
	/******************************************************************/
	/*		Task 1 	-  Wave drive mode 360 counter clockwise rotation 	*/
	/******************************************************************/

	while(1){
			// decrements stepper (counter clockwise rotation)
			GPIOB->DATA = (GPIOB->DATA & ~0x0F) | wave_step[i]; 								// turns off signal to stepper (with PORTF) and asserts appropriate step to the motor
			delayUs(1200); 
			i = (i - 1 + 4) % 4; 


	GPIOB->DATA &= 0x00;
	
		/**********************************************************/
   	/*		Task 2 	-	 Wave drive mode 360 clockwise rotation		*/
		/**********************************************************/

			//increments stepper (clockwise rotation)
			GPIOB->DATA = (GPIOB->DATA & ~0x0F) | wave_step[i]; 
			delayUs(1200); 
			i = (i + 1) % 4;	
			

	/**************************************************************************/	
	/*		Task 3	-	 Full drive mode counter-clockwise and clockwise rotaton	*/
	/**************************************************************************/
		

		
				//increments stepper (clockwise rotation)
				GPIOB->DATA = (GPIOB->DATA & ~0x0F) | full_step[i]; 
				delayUs(1200); 
				i = (i + 1) % 4;	
	
				// decrements stepper (counter-clockwise rotation)
				GPIOB->DATA = (GPIOB->DATA & ~0x0F) | full_step[i]; 
				delayUs(1200); 
				i = (i - 1 +4) % 4; 
			
			

	/**************************************************************************/
	/*		Task 4	-	 Half drive mode counterclockwise and clockwise rotaton		*/
	/**************************************************************************/		
				//increments stepper (clockwise)
				GPIOB->DATA = (GPIOB->DATA & ~0x0F) | half_step[i]; 
				delayUs(1200); 
				i = (i + 1) % 8;	
		
				// decrements stepper (counterclockwise)
				GPIOB->DATA = (GPIOB->DATA & ~0x0F) | half_step[i]; 
				delayUs(1200); 
				i = (i - 1 + 8) % 8; 
			
			
	}
}

/*Delay in number of microseconds with 50MHz clock speed */
void delayUs(int n)
{
 int a, b;
 for (a = 0; a < n; a++)
 for (b = 0; b < 6; b++) {} 
}
