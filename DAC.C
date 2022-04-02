// DAC.c
// This software configures DAC output
// Lab 6 requires 6 bits for the DAC
// Runs on LM4F120 or TM4C123
// Program written by: ASHWIN RAM

// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data



// **************DAC_Init*********************
// Initialize 6-bit DAC, called once
// Input: none
// Output: none
void DAC_Init(void){

  	SYSCTL_RCGCGPIO_R|=0X02; //SET PORT B
  	__nop(); //stabilize clock
    	__nop();

 	//set the direction register for port 5-0 (port b outputs)
 	GPIO_PORTB_DIR_R|=0X3F; //set bits 5-0 to be outputs
 	GPIO_PORTB_DEN_R|=0x3F; //DIGITALLY ENABLE PINS 5-0
 	GPIO_PORTB_DR8R_R |= 0x3F; //ENABLE THE 8MA DRIVE (MORE CURRENT INTO RESISTOR)
 	 
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
   	GPIO_PORTB_DATA_R = data;

}
