/////////////////////////////////////////////////////////////////////////
// User  Longtitude and LAatitude ( provided by app)
// Latitude   (approx.) : 33.783764
// Longtitude (approx.) : -118.110426
//
// time must be in civilian time , not military time
// UTC = PST + 7 or if not dalyight savings ime UTC = PST + 8
// 
// 
//
/////////////////////////////////////////////////////////////////////////
// Alitude (alt) : the up and down of the scope
// Azimuth (az)  : the left and right of the base of the telescope mount
/////////////////////////////////////////////////////////////////////////
#include "StarLord.h"



float n ; 

/////////////////////
// Global Variables
/////////////////////
float DJD200 ; // Days from Julian Date 

//////////////////////////////////////////////////
// Calculate Days since Julian year 2000
// Input : Universal time hours (ut_hr)
//				 Universal time minutes (ut_mr)
//         Current days since begning of year to first day of month ( MDecr)
//         Current days since beginning of the month (Doffsetr)
//         Current Days since the first day of year 2000 ( Jan 1)
// Output : Days that have passed since Jan 1, 2000
//////////////////////////////////////////////////////
float J2000Days( int ut_hr , int ut_mr, float MDecr , float Doffsetr , float jyearr)
{
 return ( ((ut_hr+(ut_mr/60))/24) + MDecr+ Doffsetr+jyearr);
}


///////////////////////////////////////////////////////////////////////////////
// Universal Time Converter to decimal   ( ONLY PST )
// Input : hrs - current hour (STD)
//				mins - current minutes (STD)
//				dst  - (1) if daylight savings time , (0) if not daylight savings time
// Output : Universal time in decimal form
////////////////////////////////////////////////////////////////////////////////
float UTC ( float hrs , float mins , int dst)
{
	float u_hrs  = hrs;
	
	 u_hrs = u_hrs + 8 ;// to convert to Universal time
	if ( u_hrs > 24 )
	{
		u_hrs  = u_hrs -24;
	}
		return  (u_hrs+(float)(mins/60));
	
	
}

//////////////////////////////////////////////////////////////////////////////
// LOCAL SIDREAL TIME : ( best explanation for it)
// Suppose you have a sunny morning. Put a stick in the ground, and watch the shadow. 
// The shadow will get shorter and shorter - and then start to get longer and longer. 
// The time corresponding to the shortest shadow is your local noon. 
// We reckon a Solar day as (roughly) the mean time between two local noons, 
// and we call this 24 hours of time.
// Input :   longtitude -- current locations longtitude
//           jday       -- Julian days since JD2000
//           ut_dec     -- Current time in Universal time in decimal form
// Output: local sideral time for the spicified location and , time and jday
////////////////////////////////////////////////////////////////////////////////
float LST(float longtitude , float jday, float ut_dec)
{
	n =(100.46 + 0.98567*jday-longtitude + 15*(ut_dec));
	if ( n > 360 )
	{
	do 
	{
		n = n- 360; 
	} while ( n > 360);
	}
	return (n <0) ? (n + 360.0) : n;
}
//////////////////////////////////////////////////////////////////////////////
// Hour Angle: 
// used in the equatorial coordinate system to give the direction of a 
// point on the celestial sphere.
//
// HR must be positive 
// If HR is negative , add 360 tobe in the 0 to 360 range.
// RA must be in degrees
// Input : LST - Local Sidreal time 
//         RAr  - Right Ascnesion
// Output: Hour Angle
//////////////////////////////////////////////////////////////////////////////

float HS ( float RAr, float LST)
{
	float HS  = LST - RAr;
	if ( HS < 0)
	{
		return (HS + 360.0);
	}
	else
	{
		return HS;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// HOUR and DECLINATION to ALT 
// Calculate the Altitude from 
// Hour angle and Declinaton
// Input : Hour Angle  - HA
//         Declination - DEC
//				 Latitude    - LAT
// Output : Altitude
//////////////////////////////////////////////////////////////////////////////////
float ALT( float HA, float DECr , float LAT)
{
  float ALT;
	ALT  =  (sinf(DECr * 0.0174533)*sinf(LAT* 0.0174533))+(cosf(DECr* 0.0174533)*(cosf(LAT*0.0174533)*cosf(HA*0.0174533)));
	// input is always in radians , output of sinf is in radians 
	ALT  =  asinf(ALT)/ 0.0174533; // division by  0.017533 gets the radians back to degrees
	return ALT;
}
////////////////////////////////////////////////////////////////////////////////////
// HOUR and DECLINATION to AZ
// Calculate th e Azimuth from Hour angle and declination
// Hour angle and Declinaton
// Input : Hour Angle  - HA
//         Declination - DEC
//				 Latitude    - LAT
//				 Altitude    - ALT
// Output : Azimuth
///////////////////////////////////////////////////////////////////////////////////
float AZ ( float HA  , float DECr, float LAT, float ALT)
{
	float AZ;
	AZ  = (sinf(DECr * 0.0174533) - (sinf(ALT * 0.0174533)*sinf(LAT * 0.0174533)))/ (cosf(ALT * 0.0174533)*cosf(LAT * 0.0174533));
	
	if( sinf(HA * 0.0174533) <0 )
	{
			return acosf(AZ)/ 0.0174533;
	}
	else
	{
		return (360.0-((acosf(AZ))/0.0174533));
	}
	
}
