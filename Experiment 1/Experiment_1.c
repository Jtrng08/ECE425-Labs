/*  Experiment 1 Lab  */
/*  Code for tasks 1 - 10  */
/*  Group Members: John Truong, Philip Aldridge */

/* Header Files */

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include "TM4C123.h"

/*  Prototype Function  */

void delayMs(int ms);

/*  Main Function  */

int main(void){
	int pattern = 0x01; 
	int i, j, f, count, delay;
	unsigned char data;
		
	/*  Initialize Pins 0 - 3 of Port B  */
	SYSCTL->RCGCGPIO |= 0x02;		/* Enables the GPIO for Port B */
	GPIOB->DIR |= 0x0F;					
	GPIOB->DEN |= 0x0F;
	
	
	/*  Initialize Pins 0 - 3 of Port D  */
	
	SYSCTL->RCGCGPIO |= 0x08;		/* Enables the GPIO for Port D */
	GPIOD->DIR &= ~0x0F;
	GPIOD->AMSEL &= ~0x0F;
	GPIOD->DEN |= 0x0F;
	
	/*  Initialize Pins 1 - 3 of Port F  */ 
	
	SYSCTL->RCGCGPIO |= 0x020;	/* Enables the GPIO for Port F */
	GPIOF->DIR |= 0x0E;
	GPIOF->DEN |= 0x0E;
	
	/*  Infinite Loop to continuously loop the code  */
 
	/*  Task 1 turns the four led on */
	
//	for (;;) {
//		GPIOB->DATA |= 0x0F; /* turn on PB3 */
//		delayMs(500);
//		GPIOB->DATA = 0x00; /* turn off PB3 */
//		delayMs(500);
//	}
	/* Task 2 sequence 0 0 0 0 -> 0 0 0 1 -> 0 0 1 1 -> 0 1 1 1 -> 1 1 1 1 -> 0 0 0 0 ... repeat */
	
	                                    
//	for (;;) { 
//		GPIOB->DATA = 0x00;
//		delayMs(200);
//		GPIOB->DATA = 0x01;
//		delayMs(200);
//		GPIOB->DATA = 0x03;
//		delayMs(200);
//		GPIOB->DATA = 0x07;
//		delayMs(200);
//		GPIOB->DATA = 0x0F;
//		delayMs(200);

//}
	
	
	
		/* Task 3 counts binary 0 - 15 */

//		for (;;) {

//				for(count = 0; count < 16; count++){ 
//						GPIOB->DATA = (0x00 + count); /* turn off PB3 */
//						delayMs(500);

//					}
//			
//		}
//		
//		/* Task 4 Generates a ring sequence shown it through led */
//		
//		for (;;) {

//			for (i = 0x01; i <= 0x08; i <<= 1) {   // << shift 1 to the left (towards the most signifigant bit 
//        GPIOB->DATA = i;  /*Turn on one LED at a time */
//        delayMs(200);    /* Delay between LED transitions */
//    }
//}
		



//		
//		/* Task 5 Generates a johnson counter sequence shown through led */

//			for(;;) {
//			
//				for(i = 0x00;i <= 0x0F; i++ ){
//					GPIOB->DATA = i;
//					delayMs(200);
//					j = i;
//					i = i+j;
//						
//					
//				}
//				for(i = 0x0F; i >=8; i-- ){
//					GPIOB->DATA = i;
//					delayMs(200);
//					j = ~i ;
//					i = i - j - 0x10;
//					
//				}
//			
//			}
			
			
//			/* Task  6 Generate a fibonacci sequence shown through led */
//		
//		i = 0;
//		j = 1;
//		count = 0;

//		for(;;){
//			
//		
//			
//		if(count <= 1) f = count;				// in order to generate fibonacci sequence 1 1 2 3 use if statement to output 1 when count is less than or equal to 1
//		else{
//		f = i + j;											// fibonacci = first[n-1] + second[n-2] 
//		i = j;													// first equals second (output n-1) 
//		j = f;													// second equals fibonacci (output n-2)
//		}
//		
//		GPIOB->DATA = (f);				// displays on led
//		delayMs(200);
//		GPIOB->DATA = 0x00;
//		delayMs(200);										// delays for a second
//		count++;												// increments for each loop
//	
//		}
		
//		/* Task 7 Generate sequence from task 2 using push buttons shown through led */

		
//		for(;;){
//		GPIOB->DATA = 0x00;			/* start of loop turn all led off */
//			
//		data = GPIOD->DATA;			/* set the input of the switches to variable data */
//		
//			if(data & 0x01) {		
//				delay = 250;
//			}
//			else if(data & 0x02){
//				delay = 500;
//			}
//			else if (data & 0x04){
//				delay = (1000);
//			}
//			else if (data & 0x08){
//				delay = (1500);
//			}
//			else{
//				GPIOB->DATA = 0x00;
//			}
//				for(i = 0x00;i <= 0x0F; i++ ){
//					GPIOB->DATA = i;
//					delayMs(delay);
//					j = i;
//					i = i+j;
//				
//		}
//}
	
		
//		/* Task 8 Write a program that reads binary values from push button and displays in led */
//		for(;;){
//		
//		GPIOB->DATA = 0x00;			// start of loop turn all led off
//			
//		data = GPIOD->DATA;			// set the input of the switches to variable data
//			
//		GPIOB->DATA = data;			// displays the input to the led
//				
//		}
		
		/*Task 9 Write a program that blinks the RGB led in sequence R -> off-> G -> off -> B */
		
//		for(;;){
//		
//			GPIOF->DATA = 0x02; // turn on RED
//			delayMs(500);
//			GPIOF->DATA = 0x00; // turn on BLUE
//			delayMs(500);
//			GPIOF->DATA = 0x08; // turn on GREEN
//			delayMs(500);
//			GPIOF->DATA = 0x00; // turn on BLUE
//			delayMs(500);
//			GPIOF->DATA = 0x04; // turn on BLUE
//			delayMs(500);
//			GPIOF->DATA = 0x00; // turn on BLUE
//			delayMs(500);
//		

//		}
		
//		/*Task 10 Write a program that blinks the RGB led in every possible sequence */    	
//		
//		for(;;){
//			

//			for(i = 0x00; i < 14; ){
//				
//				i = i + 2;
//				GPIOF->DATA = i;
//				delayMs(500);
//				
//	
//			}
//		}
	
	return 0;	
}

void delayMs(int ms){									/*  Delay Function  */
	int i, j;
		for(i = 0 ; i< ms; i++)
			for(j = 0; j < 6265; j++)
				{} /* do nothing for 1 ms */
}