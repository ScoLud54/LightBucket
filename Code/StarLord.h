///////////////////////////////////////////////////////
// Project: Light Bucket
// Scope Type: Table Top Mount
// MOUNT TYPE : Alt-Az Mount
// Object being Viewed: Moon
///////////////////////////////////////////////////////

#include <math.h>

   

/////////////////////////////////////////////////////////
// Calculating RA ( Right Ascension in Degrees)
// Step 1 : convert to Decimal Hrs
// Step 2 : multilply those decimal hrs by 15 to convert to degrees
//
// (Using presentation day as example)
// ex)RA : 5h 42m 48s 
//    time to decimal hrs
//    5 + 42/60 + 48/3600 = 5.713333333
//
//    decimal hrs to degrees
//    5.713333333*15 = 85.7 <-- Right Ascension in degrees
///////////////////////////////////////////////////////////////////////////////
#define RA  86.217//69.7750// Right Ascension
#define DEC 16.43;// Declination
///////////////////////////////////////////////////////////////////////////////
////////////////////////
// Local Sideral TIme
////////////////////////
float LST(float longtitude , float jday, float ut_dec);
////////////////////////
// Azimuth Coordinates
////////////////////////
float AZ ( float HA  , float DECr, float LAT, float ALT);
////////////////////////
// Altitude Coordinates
////////////////////////
float ALT ( float HA, float DECr , float LAT);
///////////////////////
// Hour Angle
///////////////////////
float HS ( float RAr, float LST);
/////////////////////////
// Universal Time Converter
//////////////////////////
float UTC ( float hrs , float mins , int dst);
///////////////////////////
// Julian Day Converter
///////////////////////////
float J2000Days( int ut_hr , int ut_mr, float MDecr , float Doffsetr , float jyearr);



