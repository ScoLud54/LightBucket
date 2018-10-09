#include <stdio.h>
#include <stdlib.h>
#include <time.h>
////////////////////////////////////////
// 					PORT B DECLARATIONS       //
////////////////////////////////////////
//#define GPIO_PORTB_DATA_BITS_R  (*((volatile unsigned long *)0x40005000))
//#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
//#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
//#define GPIO_PORTB_IM_R         (*((volatile unsigned long *)0x40005410))
//#define GPIO_PORTB_RIS_R        (*((volatile unsigned long *)0x40005414))
//#define GPIO_PORTB_MIS_R        (*((volatile unsigned long *)0x40005418))
//#define GPIO_PORTB_ICR_R        (*((volatile unsigned long *)0x4000541C))
//#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
//#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
//#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
//#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
//#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
//#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
//#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
//////////////////////////////////////////
//			PORT D DECLARATIONS							//
//////////////////////////////////////////
//#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
//#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
//#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
//#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
//#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
//#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
//#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
//#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
	

#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value
//////////////////////////////////////////
//			CLOCK DECLARATIONS              //
//////////////////////////////////////////
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC1_UART0      0x00000001  // UART0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOB      0x00000002  // Port B Clock Gating Control
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
//////////////////////////////////////////
//           STEP RESOLUTION						//
//////////////////////////////////////////
#define FULLSTEP           1 //  1
#define HALFSTEP           2 //  1/2
#define FOUTHSTEP          3 //  1/4
#define EIGHTSTEP          4 //  1/8
#define SIXTEENTHSTEP      5 //  1/16    
#define THIRTYTWOSTEP_v0   6 //  1/32
#define THIRTYTWOSTEP_v1   7 //  1/32
#define THIRTYTWOSTEP_v2   8 //  1/32

#define FORWARD 1
#define DOWWARD 0

#define STEPA_PIN   (*((volatile unsigned long *)0x40005004))
#define DIR_PIN    (*((volatile unsigned long *)0x40005008))
#define SLEEP_PIN  (*((volatile unsigned long *)0x40005010))
#define STEPB_PIN   (*((volatile unsigned long *)0x40005020))
#define RESET_PIN  (*((volatile unsigned long *)0x40005200))
	
#define MO_PIN     (*((volatile unsigned long *)0x40007004))
#define M1_PIN     (*((volatile unsigned long *)0x40007008))
#define M2_PIN     (*((volatile unsigned long *)0x40007010))
///////////////////////////////////////////
// PIN CONNECTIONS
//
// MO    : PD0
// M1    : PD1
// M2    : PD2
// STEPA : PE0
// SLEEP : PB2
// STEPB : PE3
//
#include "Motor.h"
#include "tm4c123gh6pm.h"

void MotorInit()
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R     |= 0x00000002; // 1) activate clock for Port B
  while((SYSCTL_PRGPIO_R&0x02) == 0) {};        // allow time for clock to start
  GPIO_PORTB_AMSEL_R &= 0x00;       // 2) disable analog on Port B
  GPIO_PORTB_PCTL_R  = 0x00000000;  // 3) PCTL GPIO on PB0,PB1,PB2,PB3,PB7 
	GPIO_PORTB_DIR_R   |= 0x8F;       // 4) Output Pins : PB0,PB1,PB2, PB3, PB7
  GPIO_PORTB_AFSEL_R &= 0x00;       // 5) disable alt funct on PB7-0
  GPIO_PORTB_PUR_R   &= 0x00;       // 6) disable pull-up on Port B
  GPIO_PORTB_DEN_R   |= 0x8F;       // 7) enable digital I/O on PB0,PB1,PB2, PB3, PB7
	
}

void PortEInit()
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R     |= 0x00000010; // 1) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10) == 0) {};        // allow time for clock to start
  GPIO_PORTE_AMSEL_R &= 0x00;       // 2) disable analog on Port E
  GPIO_PORTE_PCTL_R  &= 0x00000000; // 3) No special functionality
	GPIO_PORTE_DIR_R   |= 0x1F;       // 4) Output Pins : PE0,PE1,PE2, PE3, PE7
  GPIO_PORTE_AFSEL_R &= 0x00;       // 5) disable alt funct on PB7-0
  GPIO_PORTE_PUR_R   &= 0x00;       // 6) disable pull-up on Port E
  GPIO_PORTE_DEN_R   |= 0x1F;       // 7) enable digital I/O on PE0,PE1,PE2, PE3, PE7
	
}
void MotorStepInit()
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R     |= 0x00000008; // 1) activate clock for Port D
  while((SYSCTL_PRGPIO_R&0x08) == 0) {};        // allow time for clock to start
  GPIO_PORTD_AMSEL_R &= 0x00;       // 2) disable analog on Port D
  GPIO_PORTD_PCTL_R  = 0x00000000;  // 3) PCTL GPIO on PD0-2 
	GPIO_PORTD_DIR_R   |= 0x07;       // 4) Output Pins : PD0,PD1,PD2,
  GPIO_PORTD_AFSEL_R &= 0x00;       // 5) disable alt funct on PD2-0
  GPIO_PORTD_PUR_R   &= 0x00;       // 6) disable pull-up on Port D
  GPIO_PORTD_DEN_R   |= 0x07;       // 7) enable digital I/O on PD0,PD1,PD2	
}
void MotorStepResolution(int step)
{
	switch (step)
	{
		case FULLSTEP :        GPIO_PORTD_DATA_R = 0x00; break; //M0 = LOW  | M1 = LOW  | M2 = LOW
		 
		case HALFSTEP :        GPIO_PORTD_DATA_R = 0x01; break; //M0 = HIGH | M1 = LOW  | M2 = LOW
		
		case FOUTHSTEP:        GPIO_PORTD_DATA_R = 0x02; break; //M0 = LOW  | M1 = HIGH | M2 = LOW
		
		case EIGHTSTEP:        GPIO_PORTD_DATA_R = 0x03; break; //M0 = HIGH | M1 = HIGH | M2 = LOW
 
		case SIXTEENTHSTEP:    GPIO_PORTD_DATA_R = 0x04; break; //M0 = LOW  | M1 = LOW  | M2 = HIGH
   
		case THIRTYTWOSTEP_v0: GPIO_PORTD_DATA_R = 0x05; break; //M0 = HIGH | M1 = LOW  | M2 = HIGH
		
		case THIRTYTWOSTEP_v1: GPIO_PORTD_DATA_R = 0x06; break; //M0 = LOW  | M1 = HIGH | M2 = HIGH

		case THIRTYTWOSTEP_v2: GPIO_PORTD_DATA_R = 0x07; break; //M0 = HIGH | M1 = HIGH | M2 = HIGH
		
		
		// Default is set to Half Step
		default:   GPIO_PORTD_DATA_R = 0x01; break; //M0 = HIGH | M1 = LOW  | M2 = LOW       
	}
}

/*               SYSTICK INITIATION                       */
///////////////////////////////////////////////////////////
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock 
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC;
}

//////////////////////////////////////////////////////////
void SysTick_Wait(unsigned long delay){
  volatile unsigned long elapsedTime;
  unsigned long startTime = NVIC_ST_CURRENT_R;
  do{
    elapsedTime = (startTime-NVIC_ST_CURRENT_R)&0x00FFFFFF;
   }
  while(elapsedTime <= delay);
}
// 10000us equals 10ms
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}
///////////////////////////////////////
// Motor Direction 
///////////////////////////////////////
void MotorDirection( int dirc)
{
 	DIR_PIN = 0;
}


void EnableMotor()
{
	SLEEP_PIN = 0; //enable(0), disable(1)
	GPIO_PORTB_DATA_R = 0x80; //enable(1), disable(0) CHECK THIS IF DOESNT WORK
	
}
//////////////////////////////////////////
//Base movement
//////////////////////////////////////////
void MotorMoveB()
{
	SysTick_Wait(6040);
  GPIO_PORTE_DATA_R ^=  0x8;//(~STEP_PIN & 0x08);//masking PE3 bit
}
/////////////////////////////////////////
// Scope Movement
/////////////////////////////////////////
void MotorMoveA()
{
	SysTick_Wait(6040);
	GPIO_PORTE_DATA_R ^=  0x1;//(~STEP_PIN & 0x01);//masking PE0 bit
}
///////////////////////////////////////////////////////////////
// Conversion of azimuth degrees into base motor step amounts
////////////////////////////////////////////////////////////////
int moveamount( float var)
{
	int approximate;
	approximate  =  (int)(var / .00010875 );
	approximate = (int)(approximate / 3 );
	return approximate;
}
///////////////////////////////////////////////////////////////
// Conversion of altitude degrees into scope motor step amounts
////////////////////////////////////////////////////////////////
int movescopeamount(float var) {
	
return (int)(( var / .0003618)/15);

}
