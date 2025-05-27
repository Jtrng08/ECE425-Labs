/*  Experiment 4 Lab	*/
/*  Tasks 1  */
/*  Group Members: John Truong, Philip Aldridge  */

/*  Header Files  */

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"

/* 	Global Variables	*/

const int timerSpeed [] = {15000, 30000, 65000, 80000};
volatile int speedIndex = 0;
const int red = 0x02, blue = 0x04, green = 0x08;
const int current_led [] = {red, green, blue};
volatile int ledIndex = 0;

/*  Prototype Functions  */

	void rgb_LED(void);
	void push_buttons(void);
	void GPIOD_Handler(void);
	void timer_delay(void);
	void TIMER1A_Handler(void);
	void delayMs(int ms);

/*	Main Function  */

int main(void){
	
			/*		Initialization		*/
	
	rgb_LED();					// function to initalize RGB LEDs
	push_buttons();			// function to initalize pushbuttons
	timer_delay();			// function to initalize timer register

	
	/*	Infinite Loop		*/
	
	while(1){
		
	}
	

	
}

void rgb_LED(void){
	
	SYSCTL->RCGCGPIO |= 0x20;								// enable clock to GPIOF
	GPIOF->DIR |= (red) | (green) | (blue);	// setting the direction as output
	GPIOF->DEN |= (red) | (green) | (blue);	// enabling digial function
	
}

void push_buttons(void){
	SYSCTL->RCGCGPIO |= 0x08;								// enable clock to GPIOD
				/*	Initialization for Pin 3-0 of PORT D  */
	
	GPIOD->DEN |= 0x0F;
	GPIOD->DIR &= ~0x0F;
//	GPIOD->PUR |= 0x0F;
	
//	GPIOD->AMSEL &= ~0x0F;

	
			/* Configuration for PORTD3-0 for falling edge trigger interrupt */
	
	GPIOD->IS  &= ~(0x0F); 	/* make bit 3-0 edge sensitive */
	GPIOD->IBE &= ~(0x0F); 	/* trigger is controlled by IEV */
	GPIOD->IEV &= ~(0x0F); 	/* falling edge trigger */
	
	GPIOD->ICR |= (0x0F); 	/* clear any prior interrupt */
	GPIOD->IM  |= (0x0F);  	/* unmask interrupt */

			/* NVIC Configurations*/
	NVIC->IP[3] = 1 << 5;				/*	set interrupt priority to 1	(shifts left from 0-4 since they are reserved)*/
	NVIC->ISER[0] |= (0x08);		/*	enable IRQ3		*/
}

void GPIOD_Handler(void){				//ISR Handler for GPIOD
	if(GPIOD->MIS & 0x08) {		// SW2
		//cycle through RGB LEDs
//			GPIOF->DATA = red;
		GPIOF->DATA &= ~0x0E;
		ledIndex = (ledIndex + 1) % 3;
		GPIOD->ICR |= 0x08;
	}
	else if(GPIOD->MIS & 0x04) {  //SW3
		//disable/ enable the LED

		if((GPIOF->DATA & 0x0F) == 0){
			TIMER1->CTL = 1;
			GPIOF->DATA ^= current_led[ledIndex];
		}
		else {
			TIMER1->CTL = 0;
			GPIOF->DATA ^= current_led[ledIndex];
		}
		GPIOD->ICR |= 0x04;
	}
	else if(GPIOD->MIS & 0x01) {  //SW5
		//cycle through four different speed options

		speedIndex = (speedIndex + 1) % 4;
		TIMER1->TAILR = timerSpeed[speedIndex] - 1;
		GPIOD->ICR |= 0x01;
	}
	else if(GPIOD->MIS & 0x02) {		//SW4
		//DO NOTHING

		GPIOD->ICR |= 0x02;
	}
		
}

	
void timer_delay(void){
	SYSCTL->RCGCTIMER |= (0x02); // enables clock for timer 1
	TIMER1->CTL = 0; 						 // disables the timer1 output
	TIMER1->CFG = 0x4;					 // selects 16-bit configuration
	TIMER1->TAMR = 0x02;				 // selects one-shot mode of timer1
	TIMER1->TAPR = 250-1;				 // loads in TimerA prescaler value
	TIMER1->TAILR = 64000-1;		 // loads in TimerA counter starting count down
	TIMER1->ICR = 0x1;				   // Timer A timeout flag bit clears
	TIMER1->IMR |=(0x01);				 // Enables TimerA time-out interrupt mask
	TIMER1->CTL |= 0x01;				 // enables the timer1 output
	NVIC->ISER[0] |= (1 << 21);	 // enable the IRQ number for Timer1A
}

void TIMER1A_Handler(void){

 if (TIMER1->MIS & 0x1) {
		GPIOF->DATA ^= current_led[ledIndex];  
		TIMER1->ICR = 0x1; /* Timer1A timeout flag bit clears*/
 }
}

void delayMs(int ms){									/*  Delay Function  */
	int i, j;
		for(i = 0 ; i< ms; i++)
			for(j = 0; j < 6265; j++)
				{} /* do nothing for 1 ms */
			}


