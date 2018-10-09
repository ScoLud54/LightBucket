////////////////////////////////////////////////////////////////////////////////
//										DRV8825 BREAKOUT BOARD PIN OUT													//
//
// 1) EN:  Name: Enable 
//			            (Negative Logic)
//			HIGH-Disable
//			LOW-Enable*
//
// 2) MO,M1,M2 : Step Resolution Setting
//
// 3) RESET: Enable or Disable H Bridge Output
//			LOW-Disbale*
//			HIGH-Enable
//
// 4) SLEEP: ENable/Disable low power sleep mode
//			LOW-Disbale*
//			HIGH-Enable
// 5) STEP: LOW -> HIGH, move one step
//
// 6) LOW / HIGH switches direction
//
// 7) VMOT: Motor Power (12- 24v)
//
// 8) FAULT : unused (?)
/////////////////////////////////////////////////////////////


///////////////////////////////////////////////////
//             Motor Initillization
// -- Setup pins for motor connections
// Input  : none
// Output : none
///////////////////////////////////////////////////
void MotorInit(void);


///////////////////////////////////////////////////
//           STEP PINS INITILIZATION
// - 3 pins needed to select step resolution
// - all 3 pins need to be output pins
void MotorStepInit(void);
	


///////////////////////////////////////////////////
//               STEP RESOLUTION
// DV8885 Breakout Board step resoluation pins :
//		M0 ,  M1, M2 
//
// Step Angle Logic: 
// M0   |  M1  | M02  | Resolution
// --------------------------------
// Low  |  Low | Low  | Full step
// High |  Low | Low  | Half step
// Low  | High | Low  | 1/4 step
// High | High | Low  | 1/8 step
// Low  |  Low | High | 1/16 step
// High |  Low | High | 1/32 step
// Low  | High | High | 1/32 step
// High | High | High | 1/32 step
///////////////////////////////////////////////////
void MotorStepResolution(int);
	


///////////////////////////////////////////////////
//       MOTOR DIRECTION
///////////////////////////////////////////////////
void MotorDirection(int);
	


///////////////////////////////////////////////////
//       MOVING OF MOTOR
// - signal the DRV8825 to begin motor stepping
///////////////////////////////////////////////////
void MotorMoveB(void);
void MotorMoveA(void);
int movescopeamount(float );//
///////////////////////////////////////////////////
//        converting degrees to how many time to 
//        0scillate pin 
////////////////////////////////////////////////////
int moveamount(float);

///////////////////////////////////////////////////
//      ENABLE DRV8825 
// - set enable 
// - disable reset
// - disable sleep
///////////////////////////////////////////////////
void EnableMotor(void);


void PortEInit(void);
/////////////////////////////////////////////////
// 			Intialize Systic
//- used to st up the timing per step
////////////////////////////////////////////////
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
