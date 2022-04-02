// Music.c
// playing your favorite song.
//
// For use with the TM4C123
// EE319K lab6 extra credit
// Program written by: ASHWIN RAM AND AVYAY RAMANATHAN
// 1/11/22

#include "Sound.h"
#include "DAC.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"


// external signal connected to PD4 (CCP0) (trigger on rising edge)




void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);	// previous I bit, disable interrupts
void EndCritical(long sr);	// restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);   // user function



void Timer0A_Init(uint32_t period){
    
    long sr;
  sr = StartCritical();
    
    period = period * 80 * 10 * 10 * 10; //get the number of ticks
    
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
 
  TIMER0_CTL_R = 0x00000000;	// 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;	// 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;	// 4) reload value
  TIMER0_TAPR_R = 0;        	// 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;	// 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;	// 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;       	// 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;	// 10) enable TIMER0A
  EndCritical(sr);
}


void Music_Init(void){
 	Timer0A_Init(500); //initialize to quarternote period
 	TIMER0_IMR_R = 0x00000000;	// disarm timeout interrupt
}

//QUARTER NOTES WAIT 500 MS; HALF NOTE WAIT 1000 MS

 	#define anote 1420 //period for a note
 	#define bnote 1265
 	#define cnote 1194
 	#define dnote 2128
 	#define enote 1896
   	#define fnote 1790
   	#define gnote 1594

 	//DURATIONS
 	#define quarter 500
 	#define half 1000

 	struct Song{
   	 	uint32_t note;
   		   uint32_t duration;
   	 
   	  };
   	 
   	  typedef struct Song Song_t;
   	 
   	  Song_t Twinkle[42] = {{cnote, quarter}, {cnote, quarter}, {gnote, quarter}, {gnote, quarter}, {anote, quarter}, {anote, quarter}, {gnote, half},
                       	{fnote, quarter}, {fnote, quarter}, {enote, quarter}, {enote, quarter}, {dnote, quarter}, {dnote, quarter}, {cnote, half},
   												  {gnote, quarter}, {gnote, quarter}, {fnote, quarter}, {fnote, quarter}, {enote, quarter}, {enote, quarter}, {dnote, half},
   												  {gnote, quarter}, {gnote, quarter}, {fnote, quarter}, {fnote, quarter}, {enote, quarter}, {enote, quarter}, {dnote, half},
   												  {cnote, quarter}, {cnote, quarter}, {gnote, quarter}, {gnote, quarter}, {anote, quarter}, {anote, quarter}, {gnote, half},
                       	{fnote, quarter}, {fnote, quarter}, {enote, quarter}, {enote, quarter}, {dnote, quarter}, {dnote, quarter}, {cnote, half}};

void turnOnAllSound(){ NVIC_ST_CTRL_R |= (0x01); }
	 
// Play song, while button pushed or until end
  void Music_PlaySong(void){
            turnOnAllSound();    	 
            TIMER0_CTL_R = 0x00000001; //enable timer
   	 TIMER0_IMR_R = 0x00000001; //arming timer0
}


	void Music_Start(uint32_t period){
  if(period==0){
    NVIC_ST_CTRL_R &= (~0x01);
    TIMER0_CTL_R = 0x00000000; //turning off the timer
    TIMER0_IMR_R = 0x00000000;  //disarming timer0
    

}
else{

    period = period * 80 * 10 * 10 * 10; //get the number of ticks
    NVIC_ST_CTRL_R |= (0x01); //turn on all sound by setting bit 1!
    TIMER0_TAILR_R = period-1;	// 4) reload value
    TIMER0_CTL_R = 0x00000001; //enable timer
    TIMER0_IMR_R = 0x00000001; //arming timer0

}


}
// Stop song
void Music_StopSong(void){
    NVIC_ST_CTRL_R &= (~0x01); //turn off all sound by clearing bit 0!
    TIMER0_CTL_R = 0x00000000; //turning off the timer
    TIMER0_IMR_R = 0x00000000;  //disarming timer0
 
}

uint32_t sidx = 0;   													 
void Timer0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
      Sound_Start(Twinkle[sidx].note); //note starts playing
	Music_Start(Twinkle[sidx].duration); //waits
      sidx = (sidx+1)%42;
}

