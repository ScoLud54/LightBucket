#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Motor.h"
#include "StarLord.h"
#include "UART.h"

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


float LSTm = 0.0; //Local siderial time
float HAm  = 0.0; //Hour Angle
float ALTm = 0.0; //Altitude
float Azm  = 0.0; //Azimuth
int i;
int hours, minutes, seconds;
float lat, longtitude, time, jd;
float utime = 0.0;
int main (void)
{
	
	////////////////////////////////////
	// INTILIAZE MOTOR
	////////////////////////////////////
	PLL_Init();
	UART0_Init();
	UART1_Init();
	PortEInit();
	MotorStepInit();
	EnableMotor();
	MotorStepResolution(SIXTEENTHSTEP);
	SysTick_Init();
  ///////////////////////////////////////
	// CALCULATIONS FOR ALT AND AZ COORDS
	/////////////////////////////////////
	
	
	/////////////////////////
	//ANDROID SENT DATA
	///////////////////////////
	
		jd         = UART1_InUFloatX();
		lat        = UART1_InUFloatX();
		longtitude = UART1_InUFloatX();
		time       = UART1_InUFloatX();
		time       = UART1_InUFloatX();

		getTimeBreak(&hours, &minutes, &seconds, time);
		
		
		//////////////////////////////////////////////////////////
		//CONVERSION OF DATA INTO USABLE UNITS
		/////////////////////////////////////////////////////////
		
								//long     ,   ,hours 
		utime = UTC(hours,minutes,1);
		LSTm  = (LST(longtitude ,jd , utime)); // calculate local sideral time 
					 //
		HAm = HS(RA, LSTm);
							  //c ,objects declination   , lat
		ALTm = (ALT(HAm,18.3208, lat));
						//HAm, objects declination, lat, altm
		Azm = AZ (HAm, 18.3208, lat, ALTm);
	
		//////////////////////////////////////////////////////////////
		// MOVE MOTORS TO SPECIFIED ALTITUDE (ALTm) AND AZIMUTH (AZm)
		//////////////////////////////////////////////////////////////
		int Amount_Move_B  =  moveamount(Azm);
		
		for ( i = 0  ; i < (Amount_Move_B  -1) ; i = i+1)
		{
			MotorMoveB();
		}
		
		int Amount_Move_Scope = movescopeamount(ALTm);
		for ( i = 0  ; i < (Amount_Move_Scope) - 1 ; i= i +1)
		{
			MotorMoveA();
		}
	return 1;
	
}


