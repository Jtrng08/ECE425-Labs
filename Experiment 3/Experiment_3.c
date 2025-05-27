/*  Experiment 3   */
/*  Tasks 1 - 4  */
/*  Group Members: John Truong, Philip Aldridge  */

/*  Header Files  */

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"
#define loadValue 62500.00

/*	Prototype Function	*/

void delayMs(int ms);
void dutyCycle(unsigned char data);
void determineDuty(double duty); 
void runServo(int speed);

/*  Main Function  */

int main(void){
	
	unsigned char data;
	int speed, pw;
	
	/*		Initialization		*/
	
	SYSCTL->RCGCPWM  |= 0x02;		// enable clock to PWM1	
	SYSCTL->RCGCGPIO |= 0x20;		// enable clock to GPIOF
	SYSCTL->RCGCGPIO |= 0x08;		// enable clock to GPIOD
	
	SYSCTL->RCC |= 0x00100000;	// enable clock divider for PWM
	SYSCTL->RCC &= ~0x000E0000;	// clear clock divide
	SYSCTL->RCC |= 0x00060000;	// set clock divide to 16 by 50 MHz/16 = 3.125 Mhz
	
	delayMs(1);									// wait for PWM clock to stabilize

/*				Generator A		Tasks 1-2		*/
//	PWM1->_3_CTL = 0;						// disable PWM_3 during configuration	
//	PWM1->_3_GENA = 0x0000008C;
//	PWM1->_3_LOAD = 62500 - 1;	// 50 Hz
//	PWM1->_3_CMPB = 61875 - 1;	// 5% duty cycle: 59375,  10% duty cycle: 56250	
//	PWM1->ENABLE |= 0x40;				// enable PWM1
	
/*				Generator B		Tasks 3-4		*/

	PWM1->_3_CTL = 0;						// disable PWM_3 during configuration
	PWM1->_3_GENB = 0x0000080C;	// output high when load and low when match
	PWM1->_3_LOAD = 62500 - 1;	// 50 Hz
//	PWM1->_3_CMPB = 61875 - 1;	// 5% duty cycle: 59375,  10% duty cycle: 56250
	PWM1->_3_CTL = 1;						// enable PWM1_3 set counter mode to count down
	PWM1->ENABLE |= 0x80;				// enable PWM1	
	
//	/*  Initialization for Pin 2 of PORT F	*/
//	GPIOF->DIR |= 0x04;					// set PORTF pin 2 as output pin
//	GPIOF->DEN |= 0x04;					// set PORTF pin 2 as digital pin
//	GPIOF->AFSEL |= 0x04;				// enable alternte function	
//	GPIOF->PCTL &= ~0x00000F00;	// clear PORTF 2 alternate function
//	GPIOF->PCTL |= 0x00000500;	// set PORTF 2 alternate function to PWM
	
	/*  Initialization for Pin 3 for PORT F		*/
	GPIOF->DIR |= 0x08;					// set PORTF pin 3 as output pin
	GPIOF->DEN |= 0x08;					// set PORTF pin 3 as digital pin
	GPIOF->AFSEL |= 0x08;				// enable alternte function	
	GPIOF->PCTL &= ~0x0000F000;	// clear PORTF 3 alternate function
	GPIOF->PCTL |= 0x00005000;	// set PORTF 3 alternate function to PWM
	
		/*	Initialization for Pin 3-0 of PORT D  */
		
	GPIOD->DIR &= ~0x0F;
	GPIOD->AMSEL &= ~0x0F;
	GPIOD->DEN |= 0x0F;
	
	/*  Infinite Loop to continuously loop the code  */

								/*  Task 1 LED with variable duty cycles  */
//	for(;;){
//	
//	data = GPIOD->DATA;
//	dutyCycle(data);
//	}

								/*	Task 2 Breathing LED	*/
	
//	for(;;){
//		for (pw = loadValue; pw > 250; pw-=250 ){		// dims led
//			PWM1->_3_CMPA = pw;
//			delayMs(5);
//		}
//		for(pw = 250; pw < loadValue; pw+=500 ){		// brightens led
//			PWM1->_3_CMPA = pw;
//			delayMs(5);
//		}
//	}
	
								/*	Task 3 Controlling servo motor	*/
	
//	for(;;){
//		for (pw = loadValue; pw >56250; pw-=50 ){		// slows motor
//			PWM1->_3_CMPB = pw;
//			delayMs(5);
//		}
//		for(pw = 56250; pw < loadValue; pw+=50 ){		// speeds up motor
//			PWM1->_3_CMPB = pw;
//			delayMs(5);
//		}
//	}
	
		/*  Task 4 Controlling Servo Motor Speed  */
		for(;;){

		data = GPIOD->DATA;
		switch(data){
		case 0x01:
			speed = 15;								// SW5: slow
			runServo(speed);
			break;
		case 0x02:
			speed = 10;							// SW4: medium
			runServo(speed);
			break;
		case 0x04:
			speed = 5;							// SW3: moderately fast
			runServo(speed);
			break;
		case 0x08:
			speed = 2;							// SW2: fast
			runServo(speed);
			break;
		}
		
	}
	
	return 0;
}

void delayMs(int ms){									/*  Delay Function  */
	int i, j;
		for(i = 0 ; i< ms; i++)
			for(j = 0; j < 6265; j++)
				{} /* do nothing for 1 ms */
			}

void dutyCycle(unsigned char data){
	double dutyCyc;
		switch(data){
		case 0x01:
			dutyCyc = 100.00;						// duty cycle 100%
			determineDuty(dutyCyc);
			break;
		case 0x02:
			dutyCyc = 50.00;							// duty cycle 50%
			determineDuty(dutyCyc);
			break;
		case 0x04:
			dutyCyc = 20.00;							// duty cycle 20%
			determineDuty(dutyCyc);
			break;
		case 0x08:
			dutyCyc = 1.00;							// duty cycle 1%
			determineDuty(dutyCyc);
			break;
		}

}

void determineDuty(double cycle){ /*		Determines the duty for Task 1		*/
	double duty;
	delayMs(1);									// wait for PWM clock to stabilize
	duty = ((100.00 - cycle)/100.00) * loadValue;
	
	PWM1->_3_CTL = 0;													// disable PWM_3 during configuration
	PWM1->_3_GENA = 0x8C;							// output high when load and low when match
	PWM1->_3_LOAD = loadValue - 1.00;						// 50 Hz
	PWM1->_3_CMPA = duty - 1.00;	  							// (100% - required duty cycle) of load value
	PWM1->_3_CTL = 1;						// enable PWM1_3 set counter mode to count down
	PWM1->ENABLE |= 0x40;				// enable PWM1									
	
} 

void runServo(int speed){		/*		Runs servo for Task 4		*/
	
	int pw;
		for(pw = 56250; pw < loadValue ; pw+= 100 ){		// speeds up motor
			PWM1->_3_CMPB = pw;
			delayMs(speed);
		}
		for (pw = loadValue; pw > 56250; pw-= 100 ){		// slows motor
			PWM1->_3_CMPB = pw;
			delayMs(speed);
		}
	

}