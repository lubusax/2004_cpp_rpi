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
#include <string.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>     // needed to define I2C_SLAVE (0x0703)
#include <linux/i2c-dev.h> // needed to define I2C_SLAVE (0x0703)

#include "Display_SH1106.h"

int main (void)
{
  Display_SH1106 display;

  display.init();

  display.sendCommand(SH1106_DISPLAYON, SH1106_DISPLAYALLON_RESUME);

  display.clearDisplay();

  nanosleep((const struct timespec[])
    {{  3          /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);
  //

  display.sendCommand(SH1106_DISPLAYON, SH1106_DISPLAYALLON_RESUME);

  char        str[3000] = LOGO_ADAFRUIT;
  char *      pStr = str;

  display.fillFullScreen(pStr);

  nanosleep((const struct timespec[])
    {{  30          /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);
  //

  display.sendCommand(SH1106_DISPLAYOFF, SH1106_NOP);
  
  // printf("Close Driver, ie release I2C bus access\n");
  close(display.getFileDevice());

  return 0;
}