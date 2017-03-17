/*****************************************************************************
 *   The proposed solution for LPCXpresso Experiment Kit, Lab 12a
 *
 *   Copyright(C) 2013, Embedded Artists AB
 *   All rights reserved.
 *
 ******************************************************************************/

#include "LPC11xx.h"
#include "type.h"
#include "board.h"
#include "gpio.h"
#include "delay.h"
#include "adc.h"
#include "pwm.h"
#include "spi.h"
#include <stdio.h>


#define DEBUG 0
#define RESET for(i =0; i<6;i++)for(j=0; j<6;j++)onMatrix[i][j]=0;dog_pos = 23;onMatrix[2][3]=2;break

const  int redMatrix[6][6] =
		  {{200*1000 + 302, 200*1000 +110, 200*1000 + 111, 200*1000+207, 200*1000 + 209, 200*1000 + 303},
		   {201*1000 + 302, 201*1000 +110, 201*1000 + 111, 201*1000+207, 201*1000 + 209, 201*1000 + 303},
	       {202*1000 + 302, 202*1000 +110, 202*1000 + 111, 202*1000+207, 202*1000 + 209, 202*1000 + 303},
		   {203*1000 + 302, 203*1000 +110, 203*1000 + 111, 203*1000+207, 203*1000 + 209, 203*1000 + 303},
		   {108*1000 + 302, 108*1000 +110, 108*1000 + 111, 108*1000+207, 108*1000 + 209, 108*1000 + 303},
		   {300*1000 + 302, 300*1000 +110, 300*1000 + 111, 300*1000+207, 300*1000 + 209, 300*1000 + 303}};

const int greenMatrix[6][6] =
		  {{200*1000 + 301, 200*1000 +006, 200*1000 + 109, 200*1000+206, 200*1000 + 208, 200*1000 + 210},
		   {201*1000 + 301, 201*1000 +006, 201*1000 + 109, 201*1000+206, 201*1000 + 208, 201*1000 + 210},
		   {202*1000 + 301, 202*1000 +006, 202*1000 + 109, 202*1000+206, 202*1000 + 208, 202*1000 + 210},
		   {203*1000 + 301, 203*1000 +006, 203*1000 + 109, 203*1000+206, 203*1000 + 208, 203*1000 + 210},
		   {108*1000 + 301, 108*1000 +006, 108*1000 + 109, 108*1000+206, 108*1000 + 208, 108*1000 + 210},
		   {300*1000 + 301, 300*1000 +006, 300*1000 + 109, 300*1000+206, 300*1000 + 208, 300*1000 + 210}};
const int reset[6][6] = { {0} };
int onMatrix[6][6] = { {0} };
int smileyMatrix[6][6] =
{{0,0,1,1,0,0},
 {0,1,0,0,1,0},
 {1,0,0,0,0,1},
 {0,0,0,0,0,0},
 {0,1,0,0,1,0},
 {0,0,0,0,0,0},
};
int frowneyMatrix[6][6] =
{{1,0,0,0,0,1},
 {0,1,0,0,1,0},
 {0,0,1,1,0,0},
 {0,0,0,0,0,0},
 {0,1,0,0,1,0},
 {0,0,0,0,0,0},
};

void setPin(int info, int on) {
	// on == 1 -> turn on
	if(on){
	  GPIOSetValue((info/1000)/ 100, (info/1000)%100, 0); //row
	  GPIOSetValue((info%1000)/ 100, (info%1000)%100, 1); //col
	}
	else{
	  GPIOSetValue((info%1000)/ 100, (info%1000)%100, 0); //col
	  GPIOSetValue((info/1000)/ 100, (info/1000)%100, 1); //row

	}

}

int moveDoge(int o[6][6], int dogePos){
	int tries = 0;

	int r = rand() %4;
	if(r == 0){
		if(DEBUG)printf("Down\n");
		// Down
		if(o[dogePos/10-1][dogePos%10] == 0)
			return dogePos-10;
		else {tries++; r = 1;}
	}
	if(r == 1){
		// Up
		if(DEBUG)printf("Up\n");
		if(o[dogePos/10+1][dogePos%10] == 0)
			return dogePos+10;
		else r = 2;
	}
	if(r == 2){
		// Left
		if(DEBUG)printf("Left\n");
		if(o[dogePos/10][dogePos%10-1] == 0)
			return dogePos-1;
		else r =3;
	}
	if(r == 3){
		// Right
		if(DEBUG)printf("Right\n");
		if(o[dogePos/10][dogePos%10+1] == 0)
			return dogePos+1;
		else if(tries == 1){
			printf("Winner! You have slain the doge\n");
			delayMS(500);
			while(GPIOGetValue(SW2_PORT, SW2_PIN) != SW_PRESSED) printOn(smileyMatrix);
			while(GPIOGetValue(SW2_PORT, SW2_PIN) == SW_PRESSED);
			int i,j;
			for(i =0; i<6;i++)for(j=0; j<6;j++)onMatrix[i][j]=reset[i][j];
			return 99;

		}
		else return moveDoge(o,  dogePos);
	}

}

void printOn(int o[6][6]){
	int i, j;
	for(i = 0; i < 6; i++)
	{
		for(j = 0; j<6; j++){
			if(DEBUG)printf("%d ", o[i][j]);
			if(o[i][j] == 0){
				setPin(redMatrix[i][j], 0);
				setPin(greenMatrix[i][j], 0);
			}
			else if(o[i][j] == 1) {
				setPin(redMatrix[i][j], 1);
				setPin(greenMatrix[i][j], 0);
			}
			else if(o[i][j] == 2) {
				setPin(redMatrix[i][j], 0);
				setPin(greenMatrix[i][j], 1);
			}
		}
		if(DEBUG)printf("\n");
		for(j = 0; j<6; j++){
				setPin(redMatrix[i][j], 0);
				setPin(greenMatrix[i][j], 0);
				//delayMS(3);
		}
	}
	if(DEBUG)printf("\n\n");
}

int main (void)
{


  //Extra, turn buzzer pin to input
  GPIOSetDir( BUZZ_PORT, BUZZ_PIN, GPIO_OUTPUT);
  GPIOSetValue( BUZZ_PORT, BUZZ_PIN, BUZZ_OFF);

  //Set LED pins as outputs
  GPIOSetDir( PORT2, 0,  GPIO_OUTPUT); GPIOSetDir( PORT2, 1,  GPIO_OUTPUT); GPIOSetDir( PORT2, 2,  GPIO_OUTPUT); GPIOSetDir( PORT2, 3,  GPIO_OUTPUT);
  GPIOSetDir( PORT1, 8,  GPIO_OUTPUT); GPIOSetDir( PORT3, 0,  GPIO_OUTPUT); GPIOSetDir( PORT3, 1,  GPIO_OUTPUT); GPIOSetDir( PORT3, 2,  GPIO_OUTPUT);
  GPIOSetDir( PORT0, 6,  GPIO_OUTPUT); GPIOSetDir( PORT1, 10,  GPIO_OUTPUT); GPIOSetDir( PORT2, 6,  GPIO_OUTPUT); GPIOSetDir( PORT2, 7,  GPIO_OUTPUT);
  GPIOSetDir( PORT2, 8,  GPIO_OUTPUT); GPIOSetDir( PORT2, 9,  GPIO_OUTPUT); GPIOSetDir( PORT2, 10, GPIO_OUTPUT); GPIOSetDir( PORT3, 3,  GPIO_OUTPUT);
  GPIOSetDir( PORT1, 9,  GPIO_OUTPUT); GPIOSetDir( PORT1, 11,  GPIO_OUTPUT);


  // Set rows and columns not to display
  GPIOSetValue( PORT2, 0, 1); GPIOSetValue( PORT2, 1, 1); GPIOSetValue( PORT2, 2, 1); GPIOSetValue( PORT2, 3, 1); GPIOSetValue( PORT1, 8, 1);
  GPIOSetValue( PORT3, 0, 1); GPIOSetValue( PORT3, 1, 0); GPIOSetValue( PORT3, 2, 0); GPIOSetValue( PORT3, 3, 0); GPIOSetValue( PORT0, 6, 0);
  GPIOSetValue( PORT1, 10, 0); GPIOSetValue( PORT2, 6, 0); GPIOSetValue( PORT2, 7, 0); GPIOSetValue( PORT2, 8, 0); GPIOSetValue( PORT2, 9, 0);
  GPIOSetValue( PORT2, 10, 0); GPIOSetValue( PORT1, 9, 0); GPIOSetValue( PORT1, 11, 0); GPIOSetDir( SW2_PORT, SW2_PIN, GPIO_INPUT);


   int turns, rows, cols, i, j;
   printf("Enter how many moves the dog will have each turn. \n (1 is easy, 2 is medium, 3 is difficult)\n");
   scanf("%d", &turns);
   printf("You have entered: %d \n Enjoy the game! \n", turns);


  //Initialize ADC peripheral and pin-mixing
  ADCInit(4500000);  //4.5MHz ADC clock

  int dog_pos = 23;
  int ole_doge=0;
  onMatrix[2][3] = 2;

  int delete;
  while(1){
	  rows = getADC(AIN0);
	  cols = getADC(AIN1);

	  if((GPIOGetValue(SW2_PORT, SW2_PIN) == SW_PRESSED) && onMatrix[rows/170][cols/170] == 0){

		  delete = 0;
		  onMatrix[rows/170][cols/170] = 1;

		  for (i=0; i < turns; i++){
			  if((dog_pos/10 == 0) || (dog_pos%10 == 0) || (dog_pos/10 == 5) || (dog_pos%10 == 5))
			  {
				  printf("Defeat!\n");
				  delayMS(500);
				  while(GPIOGetValue(SW2_PORT, SW2_PIN) != SW_PRESSED){
					  printOn(frowneyMatrix);
				  }
				  while(GPIOGetValue(SW2_PORT, SW2_PIN) == SW_PRESSED);
				  RESET;
			  }
			  ole_doge = dog_pos;
			  dog_pos = moveDoge(onMatrix, dog_pos);

			  if(dog_pos > 90){RESET;}

			  onMatrix[ole_doge/10][ole_doge%10] = 0;
			  onMatrix[dog_pos/10][dog_pos%10] = 2;
		  }

	  }
	  else if(onMatrix[rows/170][cols/170] == 0){
		  delete = 1;
		  onMatrix[rows/170][cols/170] = 1;
	  }
	  else delete = 0;

	  printOn(onMatrix);
	  if(delete) onMatrix[rows/170][cols/170] = 0;

  }
  return 0;
}
