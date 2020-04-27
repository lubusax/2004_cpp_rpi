// Exploring BeagleBone:
// Tools and Techniques for Building with Embedded Linux
// By Derek Molloy
// page 286 I2C Communication in C

// https://elinux.org/Interfacing_with_I2C_Devices

// https://embetronicx.com/tutorials/linux/device-drivers/ioctl-tutorial-in-linux/

// https://linux.die.net/man/3/explain_ioctl

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>     // needed to define I2C_SLAVE (0x0703)
#include <linux/i2c-dev.h> // needed to define I2C_SLAVE (0x0703)
#include <iostream>
#include <fstream>
#include <iomanip>

#include "Display_SH1106.h"

#define MAX_SIZE_FILE  8000 

int main (void)
{
  Display_SH1106 display;

  display.init();

  display.sendCommand(SH1106_DISPLAYON, SH1106_DISPLAYALLON_RESUME);

  display.clearDisplay();

  nanosleep((const struct timespec[])
    {{  2          /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);
  //
  
  char charArray[] = LOGO_ADAFRUIT;
  // int size = sizeof charArray/ sizeof charArray[0];
  //std::cout << size << std::endl;
  

  display.sendCommand(SH1106_DISPLAYON, SH1106_DISPLAYALLON_RESUME);

  display.readFullScreen("testOutFile");

  //display.setFullScreen((const char *) charArray);
  display.fillFullScreen(display.getFullScreen());
  //display.writeFullScreen("testOutFile");

  nanosleep((const struct timespec[])
    {{  12         /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);
  //

  display.sendCommand(SH1106_DISPLAYOFF, SH1106_NOP);
  
  // printf("Close Driver equals to release I2C bus access\n");
  close(display.getFileDevice());

  return 0;
}