/*  Experiment 2 Lab  */
/*  Tasks 1 - 3  */
/*  Group Members: John Truong, Philip Aldridge  */

/*  Header Files  */

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"
#define M_Size 26

/*  Prototype Function  */

void delayUs(int ttime);
void delayMs(int ms);
void playNote(int freq, int duration);
	
/*  Main Function  */

int main(void){
	int idx, data, defDur = 100;
	int octFreq, C4 = 261, D4 = 293, F4 = 349, E4 = 329;
	int freq[M_Size] = {261.6, 261.6, 293.7, 261.6, 349.2, 329.6, 261.6, 261.6, 293.7, 261.6, 392.0, 349.2, 261.6, 261.6, 261.6, 440.0, 
	349.2, 329.6, 293.7, 466.2, 440.0, 349.2, 392.0, 698.5};
	int duration[26] = {75, 25, 100, 100, 100, 100, 200, 75, 25, 100, 100, 100, 200, 75, 25, 100, 100, 100, 100, 500, 75, 25, 100, 100,
	100, 200 };
	
	/*  Initialization  */
	
	/*	Initializes PC4 of Port C  */
	SYSCTL->RCGCGPIO |= 0x04;  // enables clock to GPIOC
	GPIOC->AMSEL &= ~0x10;     // disable analog function
	GPIOC->DIR |= 0x10;				 // sets PC4 as output pin
	GPIOC->DEN |= 0x10; 			 // sets PC4 as digital pin
	
	/*	Configures the Timer Block 0  */
	SYSCTL->RCGCTIMER |= 1;
	TIMER0->CTL = 0;
	TIMER0->CFG = 0x04;
	TIMER0->TAMR = 0x02;
	TIMER0->TAILR = 16 - 1;
	TIMER0->ICR = 0x01;
	TIMER0->CTL |= 0x01;
	
	/*	Initializes Pins 0 - 3 of Port D  */
	SYSCTL->RCGCGPIO |= 0x08;		// enables clock to GPIOD
	GPIOD->DIR &= ~0x0F;				// Set pins 3-0 as input pins
	GPIOD->AMSEL &= ~0x0F;			// disables analog function
	GPIOD->DEN |= 0x0F;					// Enables digital function
	
	/*  Initializes Pins 0 - 3 of Port B  */
	SYSCTL->RCGCGPIO |= 0x02;
	GPIOB->DIR |= 0x0F;
	GPIOB->DEN |= 0x0F;
	
	/*  TASKS  */
	
	
	/*  Task 1 Play the Happy Birthday Music  */
	/*  Infinite loop  */
	
	for(;;){
		
		/*  for state to loop through array for frequency and duration  */
		for(idx = 0; idx < M_Size ; idx++){			
		
			/*  Task 2 Use pushbuttons to change the octaves  */
			data = GPIOD->DATA;								// takes input of pushbuttons sets to data
			switch (data){										// switch statement to select the desired octave
				case 0x01:
					octFreq = freq[idx] * 2;			// SW5(PD0) pressed octave 5
					playNote(octFreq, duration[idx]); // function call to play the 
					break;												// multiplied by a factor of 2
				case 0x02:
					octFreq = freq[idx] * 4;			// SW4(PD1) pressed octave 6
					playNote(octFreq, duration[idx]); // function call to play the 
					break;												// multiplied by a factor of 4
				case 0x04: 
					octFreq = freq[idx] * 8;			// SW3(PD2) pressed octave 7
					playNote(octFreq, duration[idx]); // function call to play the 
					break;												// multiplied by a factor of 8
				default:
					octFreq = freq[idx];
					playNote(octFreq, duration[idx]); // For Task 2 function to play the note(s)
				}
			
 
		  playNote(freq[idx], duration[idx]);			// For Task 1 function to play the note(s)


		}
	}
	
	/*	Task 3 Create a mini piano using pushbuttons and LEDs  */
	/*  Infinite loop  */ 

		for(;;){
			data = GPIOD->DATA;			

			switch (data){
				case 0x01:							// SW5 pressed plays C4
					GPIOB->DATA = data;
					playNote(C4,defDur); 
					break;
				case 0x02:							// SW4 pressed plays D4
					GPIOB->DATA = data;
					playNote(D4, defDur);
					break;
				case 0x04:							// SW3 pressed plays F4
					GPIOB->DATA = data;
					playNote(F4, defDur);
					break;
				case 0x08:							// SW2 pressed plays E4
					GPIOB->DATA = data;
					playNote(E4, defDur);
					break;
				default:

					GPIOB->DATA = 0x0;
					break;
			}
				
		}
	return 0;
	
}

void playNote(int freq, int duration){
	int i;
	int period = 1000000/freq;								// finds T the period in micro seconds 
	int halfPeriod = period/ 2;								// finds the half period 
		for(i = 0; i < duration ; i++){	// loop to generate the sound wave

			GPIOC->DATA |= 0x10;									// turn on PC4
			delayMs(halfPeriod);									// half period
			GPIOC->DATA &= ~0x10;									// turn off PC4
			delayMs(halfPeriod);									// half period
			
		}
			delayMs(20000);												// delay to hear the distinct noises of each note
	}

void delayMs(int ttime){
	int i;
	for(i = 0; i < ttime; i++){
		while((TIMER0->RIS & 0x1) == 0); 				// wait for TimerA timeout flag
		TIMER0->ICR = 0x1;							 				// clear the TimerA timeout flag
	}
}

