/*  Experiment 7 lab */
/*  Tasks 1	 */
/*  Group Members: John Truong, Philip Aldridge  */

				/****  Header Files  ****/

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"
#include <stdio.h>
#include <ctype.h>

#define fsys 16000000
#define baudrate 9600


volatile char Rxdata;
/********** Prototype Function **********/
void general_Init(void);
void delayMs(int ms);
void UART0_Handler(void);
void UART0_TransmitChar(char c);
void UART0_TransmitString(const char * str);

/********** Main function ************/

int main(void) {
	unsigned int potent_dig, photosensor_dig, temperature_dig;
	double analog_reading;
	char reading_str[20] = { ' ' };
	
	/* 	Initalization	*/
		general_Init();
	
	while(1){

		
		
		
		ADC0->PSSI |= (1);								//starts the sampler 
		while((ADC0->RIS & 1) == 0);		// waits until sample conversion is completed
		potent_dig = ADC0->SSFIFO0;			// reads adc conversion results from he FIFO
		photosensor_dig = ADC0->SSFIFO0;
		temperature_dig = ADC0->SSFIFO0;
		
		ADC0->ISC = 1;									// clears the flag
		

		
		switch(tolower(Rxdata)){	
			case 'v':  // reads potentiometer readings
				analog_reading = (potent_dig * 3.3) / 4095;
				UART0_TransmitString("Potentiometer Reading: ");
				sprintf(reading_str, "%.6f V\r\n", analog_reading);			// converts digital value to analog value
				UART0_TransmitString(reading_str);
				break;
			case 'l':  // reads photosensor readings
				analog_reading = (photosensor_dig * 3.3) / 4095;				// converts digital value to analog value
				UART0_TransmitString("Photosensor Reading: ");
				sprintf(reading_str, "%.6f V\r\n", analog_reading);
				UART0_TransmitString(reading_str);
				break;
			case 't':	 // reads temperature readings
				analog_reading = (temperature_dig * 3.3) / 4095;					// converts digital value to analog value
				analog_reading = analog_reading * 100;									
				analog_reading = ((analog_reading * 9.0) / 5.0) + 32.0;		// converts to Fahrenheit
				UART0_TransmitString("Temperature Reading: ");
				sprintf(reading_str, "%.6f F\r\n", analog_reading);
				UART0_TransmitString(reading_str);
				break;
			case 'q':  // shows menu screen
				UART0_TransmitString("*****MAIN MENU***** \n\r");
				UART0_TransmitString("     OPTIONS:       \n\r");
				UART0_TransmitString("'v' or 'V' for potentiometer readings \n\r");
				UART0_TransmitString("'l' or 'L' for photosensor readings \n\r");
				UART0_TransmitString("'t' or 'T' for temperature readings \n\r");
				UART0_TransmitString("'q' or 'Q' for MAIN MENU \n\r");
				break;
			default:
				UART0_TransmitString("INVALID OPTION");
		
		}
		
		
		delayMs(100);
		UART0_TransmitString("\033[2J");		// "erase display" command in ANSI
		UART0_TransmitString("\033[H");			// "cursor home" command in ANSI
	
	}
}

void general_Init(void){
	SYSCTL->RCGCGPIO |= 1; // enables clock to GPIOA
	SYSCTL->RCGCUART |= 1;	// enables clock to UART0
	delayMs(5);
	
	GPIOA->AFSEL |= 3;   // enables alternate function of PA0 and PA1
	GPIOA->PCTL  |= 17;		// configures the pins for UART functionality
	GPIOA->DEN   |= 3;   // enables digital functionality of PA0 and PA1
	
	UART0->CTL &= ~1;    // disables UART0 for configuration
	UART0->IBRD = fsys / (16* baudrate);       // sets the baud rate 
	UART0->FBRD = 11;
	
	UART0->CC = 0; 					// uses system clock for UART0 module
	UART0->LCRH = 0x60;			// sets UART0 as 8-bit data, no parity, 1-stop bit
	UART0->CTL |= (1<<0)| (1<<8) | (1<<9); // enables UART0, and transmit/receive
	delayMs(50);						// delay for UART to initalize
	
	UART0->DR = 0;					// clears the UART0 data register
	UART0->IM |= (1<<4);		// enables UART Rx interrupt
	
	NVIC->IP[5] = 3<<5;			// enables interrupt with priority of 3
	NVIC->ISER[0] = 1<<5;		// enables IRQ 5 (UART0)
	UART0->ICR |= (1<<4);		// clears the flag
	
	// Configuration for ADC Readings
	
	SYSCTL->RCGCGPIO |= 0x10;	// enables clock for PORTE
	SYSCTL->RCGCADC |= 1;			// enables clock for ADC
	
	/* Configuration for Pins PE1 (Light Sensor), PE2(Potentiometer), and PE5(Temperature Sensor) */
	GPIOE->DIR &= ~0x26; // sets the direction as input
	GPIOE->DEN &= ~0x26; // disables digital function
	GPIOE->AFSEL |= 0x26; // enables analog function
	GPIOE->AMSEL |= 0x26;
	
	/* Configuration for the Analog to Digital Converter function */
	ADC0->ACTSS &= ~1; // disables sequencer
	ADC0->EMUX &= ~0x000F; // sets software trigger
	ADC0->SSMUX0 &= ~0xFFFFFFFF; // clears channel register
	ADC0->SSMUX0 |= 0x00000821; // channel to mux
	ADC0->SSCTL0 |= 0x00000600; // set interrupt 
	ADC0->ACTSS |= 0x1; // reenable clock

}

void UART0_Handler(void){
	
	if(UART0->MIS & (1 << 4)){
		Rxdata = UART0->DR;				// reads data
		UART0->ICR |= (1<<4);			// clears interrupt
	}
}

void UART0_TransmitChar(char c){
	while((UART0->FR & (1<<5))!= 0) {}	// waits until transmit buffer is not full
	UART0->DR = c;											// transmit character
}

void UART0_TransmitString(const char * str){
	while(* str != '\0'){
		UART0_TransmitChar(* str++);
	}
	
}

void delayMs(int ms){									/*  Delay Function  */
	int i, j;
		for(i = 0 ; i< ms; i++)
			for(j = 0; j < 6265; j++)
				{} /* do nothing for 1 ms */
			}