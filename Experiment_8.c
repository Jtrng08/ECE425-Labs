/*  Experiment 8 lab */
/*  Tasks 1	 */
/*  Group Members: John Truong, Philip Aldridge  */

				/****  Header Files  ****/

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include"TM4C123.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define fsys 16000000			// system clock
#define baudrate 9600			// baud rate
#define QUAD 10

volatile char square[QUAD] = {'0','1', '2', '3', '4', '5', '6', '7', '8', '9'};		// Label for Cells
volatile int cont, valid = 1, choice = 0, confirm;															// Free variables

int i, currPlayer = 1, restart = 1, winner1 = 0, winner2 = 0;
char victoryLine[35];
char displayChoice[25];
char markX = 'X', markO = 'O';
// Struct for 2 Players
struct players {
	char symbols;
	int player;
};

/********** Prototype Function **********/
void general_Init(void);
void delayMs(int ms);
void UART0_Handler(void);
void UART0_TransmitChar(char c);
void UART0_TransmitString(const char * str);

void displayGrid(void);
void markBoard(char mark);
int checkForWin(void);

void displayPlayer(struct players currPlayer);
void playGame(void);

void GPIOD_Handler(void);



/********** Main function ************/

int main(void) {
	
	// Initialization
	general_Init();
  playGame();       
	while(1){}
      

}

void general_Init(void){
	SYSCTL->RCGCGPIO |= 1; // enables clock to GPIOA
	SYSCTL->RCGCUART |= 1;	// enables clock to UART0
	delayMs(5);
	
	GPIOA->AFSEL |= 3;   // enables alternate function of PA0 and PA1
	GPIOA->PCTL  |= 17;		// configures the pins for UART functionality
	GPIOA->DEN   |= 3;   // enables digital functionality of PA0 and PA1
	
	UART0->CTL &= ~1;    // disables UART0 for configuration
	UART0->IBRD = fsys / (16 * baudrate);       // sets the baud rate 
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
	
		SYSCTL->RCGCGPIO |= 0x08;								// enable clock to GPIOD
				/*	Initialization for Pin 3-0 of PORT D  */
	
	GPIOD->DEN |= 0x0F;
	GPIOD->DIR &= ~0x0F;
	
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

//void UART0_Handler(void){
//	
//	if(UART0->MIS & (1 << 4)){
//		Rxdata = UART0->DR;				// reads data
//		UART0->ICR |= (1<<4);			// clears interrupt
//	}
//}

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

			
			/*				Displays the 3x3 grid for tic-tac-toe							*/
void displayGrid(void){
    char grid[215];				

	// Initalizes the board into char grid
  sprintf(grid,"\n\n\rTic Tac Toe\n\n\r"
         "Player 1 (X) - Player 2 (O)\r\n\n"
         "     |     |     \r\n"
         "  %c  |  %c  |  %c \r\n"
         "_____|_____|_____\r\n"
         "     |     |     \r\n"
         "  %c  |  %c  |  %c \r\n"
         "_____|_____|_____\r\n"
         "     |     |     \r\n"
         "  %c  |  %c  |  %c \r\n"
         "     |     |     \r", square[1], square[2],square[3], square[4],         
                          square[5],square[6], square[7], square[8],square[9]);
	// displays board
	UART0_TransmitString(grid);

}



void markBoard(char mark){
  // Based on the users input will replace the quadrant with the current players symbol
  // once filled if the user tries to select the choice again they will be prompted with a message
  if (choice == 1 && square[1] == '1')
  {
    square[1] = mark;
		valid = 1;
  }
  else if (choice == 2 && square[2] == '2'){
    square[2] = mark;
		valid = 1;
  }
  else if (choice == 3 && square[3] == '3'){
    square[3] = mark;
		valid = 1;
  }
  else if (choice == 4 && square[4] == '4'){
    square[4] = mark;
		valid = 1;
  }
  else if (choice == 5 && square[5] == '5'){
    square[5] = mark;
		valid = 1;
  }
  else if (choice == 6 && square[6] == '6'){
    square[6] = mark;
		valid = 1;
  }
  else if (choice == 7 && square[7] == '7'){
    square[7] = mark;
		valid = 1;
  }
  else if (choice == 8 && square[8] == '8'){
    square[8] = mark;
		valid = 1;
  }
  else if (choice == 9 && square[9] == '9'){
    square[9] = mark;
		valid = 1;
  }
  else {
    UART0_TransmitString("\n\n\rINVALID MOVE\r\n");
    valid = 0;
  }
	
}

int checkForWin(void){
  // IF a row, column, or diagonal matches then returns (1) Winner
	// IF row, column, and diagonal are not all filled and none matches returns (-1) Loops to next player
	// IF row, column, and diagonal do not match and IS filled returns (0) Draw
  int returnValue = 0;
  if (square[1] == square[2] && square[2] == square[3] )				// if row 1 is the same then that player wins 
    returnValue = 1;
  else if (square[4] == square[5] && square[5] == square[6]) 		// if row 2 is the same then that player wins
   returnValue = 1;
  else if (square[7] == square[8] && square[8] == square[9])		// if row 3 is the same then that player wins
   returnValue = 1;
  else if (square[1] == square[5] && square[5] == square[9])		// if lower diagonal is the same then that player wins
    returnValue = 1;
  else if (square[7] == square[5] && square[5] == square[3])		// if upper diagonal is the same then that player wins
    returnValue = 1;
  else if (square[1] == square[4] && square[4] == square[7])		// if column 1 is the same then that player wins
    returnValue = 1;
  else if (square[2] == square[5] && square[5] == square[8])		// if column 2 is the same then that player wins
    returnValue = 1;
  else if (square[3] == square[6] && square[6] == square[9])		// if column 3 is the same then that player wins
    returnValue = 1;
  else if(square[1] != '1' && square[2] != '2' && square[3] != '3' && square[4] != '4' &&
    square[5] != '5' && square[6] != '6' && square[7] != '7' &&	
    square[8] != '8' && square[9] != '9')
    returnValue = 0;
  else
    returnValue = -1;
  return returnValue;
}

void displayPlayer(struct players currPlayer){
	char prompt[75];
	
	// Creates a string variable for the prompt
  sprintf(prompt,"\n\rNow, Player %d turn.\n\r"
                 "Player %d, enter your choice with push button [1]: ", currPlayer.player, currPlayer.player); 
	UART0_TransmitString(prompt);
  
	// Using Pushbutton 2 if variable confirm is true then mark the board else loop back to same player and prompt wrong choice
	



}

void GPIOD_Handler(void){				//ISR Handler for GPIOD
	
	// Push button SW5 is pressed will cycle through 1-9 (cells) 
	if(GPIOD->MIS & 0x01){

		choice++;
		choice %= 10;
		sprintf(displayChoice, "\n\n\rCurrent choice: %d ", choice);
		UART0_TransmitString(displayChoice);
		UART0_TransmitString("\n\rPress push button [2] to confirm: ");	
	
		GPIOD->ICR |= 0x01;
	}
	// Push button SW4 is pressed confirms index
	else if (GPIOD->MIS & 0x02){
		
		if (currPlayer == 1) 
			markBoard(markX);
		else
			markBoard(markO);
		
		if(valid == 1){
			currPlayer++;
		}
		playGame();
		choice = 0;
		GPIOD->ICR |= 0x02;
	}
	// Push button SW3 is pressed resets game
	else if (GPIOD->MIS & 0x04){
       for (i = 0; i < QUAD; i++) {
            square[i] = '0' + i;
       }
			currPlayer = 1;
			choice = 0;
			UART0_TransmitString("\n\n\rRESETTING GAME...");
			playGame();
				
		GPIOD->ICR |= 0x04;
	}
		
}

void playGame(void){
	struct players player1 = {'X', 1};
  struct players player2 = {'O', 2};
    
			// displays the current game board	
      displayGrid();
			// Switches between players using modulo operator
      currPlayer = (currPlayer % 2) ? 1:2;
			
			// Based on the currPlayer displays their turn, and checks for winner
      if (currPlayer == 1) { 
          displayPlayer(player1);
					i = checkForWin();
          if (i == 1) { winner1 = 0; winner2 = 1;}
					// If valid choice was pressed switch player otherwise maintain current player

          } 
      else { 						  	
          displayPlayer(player2);
					i = checkForWin();
          if (i == 1) { winner2 = 0; winner1 = 1;}
					// If valid choice was pressed switch player otherwise maintain current player
 
        			} 
			
      // Switch Case for displaying Game Results															
      switch(i){
        case 1:
          if (winner1 == 1) {
            sprintf(victoryLine,"\n\n\rGame end : Winner Player %d !", player1.player);
						UART0_TransmitString(victoryLine);
            }
          else if (winner2 == 1){
            sprintf(victoryLine,"\n\n\rGame end : Winner Player %d !", player2.player);
						UART0_TransmitString(victoryLine);
          }
          break;
        case 0:
          UART0_TransmitString("\n\rGame end : Result Draw");
          break;
        case -1:
					// Valid choice -> Switch Player | Invalid Choice -> Maintains Current Player
          if (valid == 1)
            UART0_TransmitString("\n\rSwitching to next Player...");
          else
            UART0_TransmitString("\n\rPlease enter a valid move!");
          break;
      
				}
			

}