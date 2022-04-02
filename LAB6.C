// Lab6.c
// Runs on TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// EE319K lab6 starter
// Program written by: ASHWIN RAM

// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Sound.h"
#include "Key.h"
#include "Music.h"
#include "Lab6Grader.h"
// put both EIDs in the next two lines
char EID1[] = "ar67262"; //  ;replace abc123 with your EID
char EID2[] = "ar72248"; //  ;replace abc123 with your EID

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DAC_Init(void);      	// your lab 6 solution
void DAC_Out(uint8_t data);   // your lab 6 solution
uint8_t Testdata;

// lab video Lab6_voltmeter, Program 6.1
// A simple program that outputs sixteen DAC values. Use this main if you have a voltmeter.
const uint32_t Inputs[16]={0,1,7,8,15,16,17,18,31,32,33,47,48,49,62,63};
int voltmetermain(void){ uint32_t i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);    
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  while(1){           	 
	Testdata = Inputs[i];
	DAC_Out(Testdata); // your lab 6 solution
	i=(i+1)&0x0F;  // <---put a breakpoint here
  }
}

// DelayMs
//  - busy wait n milliseconds
// Input: time to wait in msec
// Outputs: none
void static DelayMs(uint32_t n){
  volatile uint32_t time;
  while(n){
	time = 6665;  // 1msec, tuned at 80 MHz
	while(time){
  	time--;
	}
	n--;
  }
}
// lab video Lab6_static. Program 6.2
// A simple program that outputs sixteen DAC values.
// Use this main if you do not have a voltmeter.
// Connect PD3 to your DACOUT and observe the voltage using TExaSdisplay in scope mode.
int staticmain(void){  
  uint32_t last,now,i;  
  DisableInterrupts();
  TExaS_Init(SCOPE);	// bus clock at 80 MHz
  LaunchPad_Init();
  DAC_Init(); // your lab 6 solution
  i = 0;
  EnableInterrupts();
  last = LaunchPad_Input();
  while(1){           	 
	now = LaunchPad_Input();
	if((last != now)&&now){
  	Testdata = Inputs[i];
  	DAC_Out(Testdata); // your lab 6 solution
  	i=(i+1)&0x0F;
	}
	last = now;
	DelayMs(25);   // debounces switch
  }
}


	 
int main(void){  	 
  DisableInterrupts();
  TExaS_Init(SIMULATIONGRADER);	// bus clock at 80 MHz
  Key_Init();
  LaunchPad_Init();
  Sound_Init();
  Music_Init();
  EnableInterrupts();

//make a heartbeat connected to portf
//we need to toggle port f pin 1
//WE NEED TO CHECK IF PA2, PA3, PA4, AND PA5 ARE PRESSED AND IF THEY ARE WE WILL OUTPUT A FREQUENCY




SYSCTL_RCGCGPIO_R|=0X20; //SET PORT F
  	__nop(); //stabilize clock
    	__nop();



//key 0 = 261.6 hz
//key 1 = 329.6 hz
//key 2 = 392.0 hz
//key 3 = 440.0 hz

uint32_t fr0 = 4778;
uint32_t fr1 = 3792;
uint32_t fr2 = 3189;
uint32_t fr3 = 2841;



  while(1){  	 
   	  	 

      	GPIO_PORTF_DATA_R ^=0x02;
      	//toggle pf1
   
     	if(Key_In()==1){ //KEY0 pa2
          	Sound_Start(fr0);
     	}
    	if(Key_In()==2){ //KEY1 pa3
          	Sound_Start(fr1);
    	}
    	if(Key_In()==4){ //KEY2 pa4
         	Sound_Start(fr2);
      	}
     	if(Key_In()==8){ //KEY 3 pa5
        	Sound_Start(fr3);
      	}
           
           //PA6
          if(((GPIO_PORTA_DATA_R &=0x40)>>6)==1){
              Music_PlaySong();
         }
 
        //pa7
           if(((GPIO_PORTA_DATA_R &=80)>>7)==1){
            Music_StopSong();
            }


     	if(Key_In()==0){
        	Sound_Start(0);
      	}
    
  }        	 
}

