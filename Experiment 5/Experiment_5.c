/*  Experiment 5 lab */
/*  Tasks 1-4  */
/*  Group Members: John Truong, Philip Aldridge  */

				/****  Header Files  ****/

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"
#include <stdio.h>


				/*****************  Prototype Functions  *****************/

		/*****************  Port initalization Functions *****************/
void PORTS_init(void);
void general_init(void);

		/*****************	LCD Configuration Functions		*****************/
void LCD_init(void);
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void displaySensor(int choice);

void delayMs(int ms);
			
			/****************		Main Function  *****************/

int main(void){
	
	/*****	Initialization	*****/
	
	general_init();
	LCD_init();
	
	/*****	Variable Declaration	*****/
	int potent_dig, photosensor_dig, temperature_dig;
	unsigned int nelms , i, sensorIndex = 0, buttonState = 0, lastButtonState = 0;
	int format = 0;
	double analog_reading;
	char reading_str[9] = {' '};
		
	while (1) {
		
	ADC0 -> PSSI |= 1; 	// starts sequencer

			
	while ((ADC0 -> RIS & 1) == 0);
			
	potent_dig 			= ADC0 -> SSFIFO0;
	photosensor_dig = ADC0 -> SSFIFO0;
	temperature_dig = ADC0 -> SSFIFO0;
		
	ADC0->ISC = 1; 												// clears interrupt flag
			
			/*********************************************************/
			/************* Task 1 Potentiometer Readings *************/
			/*********************************************************/
		
			analog_reading = (potent_dig * 3.3) / 4095;
			sprintf(reading_str, "%6f", analog_reading); 	 // outputs the calculated analog reading into string
			
			LCD_command(1); 														 	 // clears LCD
			displaySensor(0);															 // function to display name of sensor
			LCD_command(0xC0); 														 // enters next line of lcd
			delayMs(1);
			
			//displays the potentiometer on the second line of LCD
			nelms = sizeof(reading_str)/sizeof(reading_str[0]);
			for(i = 0; i<nelms; i++){ LCD_data(reading_str [i]);			}
			LCD_data('V');		
	
			delayMs(100);
			
			/*********************************************************/
			/************* Task 2 Light Sensor Readings **************/
			/*********************************************************/
			
			analog_reading = (photosensor_dig * 3.3) / 4095;
			sprintf(reading_str, "%6f", analog_reading); // outputs the calculated analog reading into string
	
			LCD_command(1); 														// clears LCD
			displaySensor(1);														// function to display name of sensor
			LCD_command(0xC0); 													// enters next line of lcd
			delayMs(1);
			
			//displays the potentiometer on the second line of LCD
			nelms = sizeof(reading_str)/sizeof(reading_str[0]);
			for(i = 0; i<nelms; i++){ LCD_data(reading_str [i]);			}
			LCD_data('V');		
	
			delayMs(100);
			/*********************************************************************************/
			/************* Task 3 Temperature Readings switched between C and F **************/
			/*********************************************************************************/
			
			buttonState = (GPIOD->DATA & 0x08) >> 3;
			
			if (buttonState != lastButtonState) { // debouncer block 
					if (buttonState == 1) {
						format = !format;
						}
					delayMs(10);
				}
			lastButtonState = buttonState;	
			
			analog_reading = (temperature_dig * 3.3) / 4095;
			analog_reading = analog_reading * 100;			//Celsius
			if(format){
				analog_reading = (analog_reading * 9.0 / 5.0) + 32.0;  //Farenheit
			}
			
			sprintf(reading_str, "%.6f", analog_reading);
			
			LCD_command(1); 														// clears LCD
			displaySensor(2);														// function to display name of sensor
			LCD_command(0xC0);
			delayMs(1);
			
			//displays the temperature on the second line of LCD
			nelms = sizeof(reading_str)/sizeof(reading_str[0]);
			for(i = 0; i<nelms; i++){ LCD_data(reading_str [i]);			}
			if(format){	LCD_data('F'); }	
			else { LCD_data('C'); }
	
			delayMs(100);
			
			/***************************************************************************/
			/************* Task 4 Sensor - based system using pushbuttons **************/
			/***************************************************************************/
			
			buttonState = (GPIOD->DATA & 0x08) >> 3;
						
			if (buttonState != lastButtonState) { // debouncer block 
					if (buttonState == 1) {
							sensorIndex++;				//increment by 1
							sensorIndex %= 3;			//loop through 3 states
						}
					delayMs(10);
				}
			lastButtonState = buttonState;					
				
			switch(sensorIndex){
				case 0:							
					// outputs Potentiometer reading
					analog_reading = (potent_dig * 3.3) / 4095;
					sprintf(reading_str, "%6f", analog_reading);
				
					LCD_command(1);														// clears LCD
					displaySensor(0);													// function to display name of sensor
					LCD_command(0xC0);  											// goes to the next line of LCD
					
					//displays the potentiometer value on second line of led
					nelms = sizeof(reading_str)/sizeof(reading_str[0]);
					for(i = 0; i<nelms; i++){ LCD_data(reading_str [i]);			}
					LCD_data('V');		
					break;
				case 1:
					// outputs Light Sensor Reading
					analog_reading = (photosensor_dig * 3.3) / 4095;
					sprintf(reading_str, "%6f", analog_reading); // outputs the calculated analog reading into string
	
					LCD_command(1); 														// clears LCD
					displaySensor(1);														// function to display name of sensor
					LCD_command(0xC0); 													// enters next line of lcd
					delayMs(1);
			
					//displays the photosensor value on the second line of LCD
					nelms = sizeof(reading_str)/sizeof(reading_str[0]);
					for(i = 0; i<nelms; i++){ LCD_data(reading_str [i]);			}
					LCD_data('V');		
					break;
				case 2:
					// outputs temperture sensor
					analog_reading = (temperature_dig * 3.3) / 4095;
					analog_reading = analog_reading * 100;			//Celsius
					analog_reading = (analog_reading * 9.0 / 5.0) + 32.0;  //Farenheit
					
					sprintf(reading_str, "%.6f", analog_reading);
		
					LCD_command(1); 														// clears LCD
					displaySensor(2);														// function to display name of sensor
					LCD_command(0xC0);													// move to new line
					delayMs(1);
			
					//displays the temperature on the second line of LCD
					nelms = sizeof(reading_str)/sizeof(reading_str[0]);
					for(i = 0; i<nelms; i++){ LCD_data(reading_str [i]);	}
					LCD_data('F'); 
					break;
//				default:
//					// any of the three cases
//					break;
				
			}
			
			delayMs(100);		

	}
	
}

		/*********************************************************/
		/**************		Function Definitions		****************/
		/*********************************************************/

void general_init(void){
	SYSCTL->RCGCADC |= 1;						// enables clock for module 1
	SYSCTL->RCGCGPIO |= 0x10; 			// enables clock to GPIOE
	SYSCTL->RCGCGPIO |= 0x08;				// enables clock to GPIOD
	
	/*		Configuration for Pins PE1 (Light Sensor), PE2(Potentiometer), and PE5(Temperature Sensor)	*/
	GPIOE->DIR &= ~0x26;			// sets the direction as input
	GPIOE->DEN &= ~0x26;			// disables digital function
	GPIOE->AFSEL |= 0x26;			// enables analog function
	GPIOE->AMSEL |= 0x26;	
	
	/*		Configuration for the Analog to Digital Converter function	*/
	ADC0->ACTSS &= ~1;							// disables sequencer
	ADC0->EMUX &= ~0x000F;					// sets software trigger
	ADC0->SSMUX0 &= ~0xFFFFFFFF; 		// clears channel register
	ADC0->SSMUX0 |= 0x00000821;			// channel to mux
	ADC0->SSCTL0 |= 0x00000600;			// set interrupt 
	ADC0->ACTSS |= 0x1; 						// reenable clock
	
	/*		Configuration for Pins PD3(SW2) 	*/
	
	GPIOD->DIR &= ~0x0F;			// sets direction as input for pin 3 - 0
	GPIOD->AMSEL &= ~0x0F;		// disables analog function
	GPIOD->DEN |= 0x0F;				// enables digital function	
	
}

void PORTS_init(void){
	
	SYSCTL->RCGCGPIO |= 0x01; // enables clock to GPIOA
	SYSCTL->RCGCGPIO |= 0x04; // enables clock to GPIOC 
	
	/*		LCD DATA Pins PA2-PA5		*/						
	GPIOA->DIR |= 0x3C;				// sets pins as output
	GPIOA->DEN |= 0x3C;				// set pins as digital
	GPIOA->AMSEL &= ~0x3C;		// turn off analog function
	GPIOA->DATA  &= ~0x3C;		// clears data register
	
	/*		LCD Enable Pin		*/
	GPIOC->DIR |= 0x40;				// sets pin as input
	GPIOC->DEN |= 0x40;				// set pins as digital
	GPIOC->AMSEL &= ~0x40;		// turn off analog function
	GPIOC->DATA  &= ~0x40;		// clears data register	
	
	/*		LCD RS (Register Select) Pin		*/
	GPIOE->AMSEL &= ~0x01; 		// disables analog function
	GPIOE->DIR |= 0x01;				// sets PE0 as output
	GPIOE->DEN |= 0x01;				// sets PE0 as digital function
	GPIOE->DATA &= ~0x01;			// sets data to high to convert incoming data into display data
}

void LCD_init(void){
	
	PORTS_init();
	/*	Initialization Sequence	*/
	delayMs(20);
	LCD_nibble_write(0x30, 0);
	delayMs(5);
	LCD_nibble_write(0x30, 0);
	delayMs(1);
	LCD_nibble_write(0x30, 0);
	delayMs(1);
	
	//Configuration
	LCD_nibble_write(0x20, 0);		// use 4-bit data mode
	delayMs(1);
	LCD_command(0x28);						//set 4-bit data, 2-line, 5x7 font
	delayMs(1);
	LCD_command(0x06);						// move cursor right
	delayMs(1);
	LCD_command(0x01);						// clear screen, move cursor to home
	delayMs(5);
	LCD_command(0x0F);						// turn on LCD
	delayMs(1);


}

void LCD_nibble_write(char data, unsigned char control){
	GPIOA->DIR |= 0x3C;		// sets PA2-PA5 as outputs
	GPIOA->DATA &= ~0x3C;	// clear the line
	GPIOA->DATA |= (data & 0xF0) >> 2; // extracts the upper nibble
	/* sets RS bit */
	if(control & 1) {GPIOE->DATA |= 1;} // check if LCD data is data
	else {GPIOE->DATA &= ~1; }					// check if LCD data is command
	delayMs(0);													// gives LCD time to react
	
	/*	send a high to low transition pulse on LCD enable pin (PC6) */
	GPIOC->DATA |= 1 << 6;
	delayMs(0);
	GPIOC->DATA &= ~(1<<6);
	delayMs(0);
	GPIOA->DATA &= ~0x3C;		// clear the line in data register
	delayMs(1);
}

void LCD_command(unsigned char command){
	LCD_nibble_write(command & 0xF0, 0); // upper nibble
	delayMs(1);
	LCD_nibble_write(command << 4, 0); 	 // lower nibble
	
	if(command < 3) {delayMs(2);}
	else {delayMs(1);}
}

void LCD_data(char data){
	LCD_nibble_write(data & 0xF0, 1);		// upper nibble
	delayMs(1);
	LCD_nibble_write(data << 4, 1);			// lower nibble
	delayMs(1);
}

void displaySensor(int choice){
	switch (choice){
		case 0:
			LCD_data('P');
			LCD_data('O');
			LCD_data('T');
			LCD_data('E');
			LCD_data('T');
			LCD_data('I');
			LCD_data('O');
			LCD_data('M');
			LCD_data('E');
			LCD_data('T');
			LCD_data('E');
			LCD_data('R');
			break;
		case 1:
			LCD_data('P');
			LCD_data('H');
			LCD_data('O');
			LCD_data('T');
			LCD_data('O');
			LCD_data('S');
			LCD_data('E');
			LCD_data('N');
			LCD_data('S');
			LCD_data('O');
			LCD_data('R');
			break;
		case 2:
			LCD_data('T');
			LCD_data('E');
			LCD_data('M');
			LCD_data('P');
			LCD_data('E');
			LCD_data('R');
			LCD_data('A');
			LCD_data('T');
			LCD_data('U');
			LCD_data('R');
			LCD_data('E');
			break;
	}
}

void delayMs(int ms){									/*  Delay Function  */
	int i, j;
		for(i = 0 ; i< ms; i++)
			for(j = 0; j < 6265; j++)
				{} /* do nothing for 1 ms */
			}