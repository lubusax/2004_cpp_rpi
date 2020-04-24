#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <wiringPiI2C.h>
#include "Display_SH1106.h"




int main (void)
{
  const int devID = 0x3C;
  Display_SH1106 display;

  wiringPiSetup () ;
  // It's actually a fatal error to call any of the 
  // wiringPiSetup routines more than once,
  // (you run out of file handles!)

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, devID);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  // result =  wiringPiI2CSetup (devID);
  pinMode (0, OUTPUT) ;

  //printf ("Result of I2C Setup %d \n", result) ;
  printf ("Did something \n") ;
  
  // for (;;)
  // {
  //   digitalWrite (0, HIGH) ; delay (500) ;
  //   digitalWrite (0,  LOW) ; delay (500) ;
  // }
  return 0 ;
}