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
//#include "FreeSansBold12pt7b.h"
//#include "FreeSansBold18pt7b.h"
//#include "FreeSansBold20pt7b.h"
//#include "FreeSansBold24pt7b.h"
//#include "FreeSans24pt7b.h"
//#include "FreeSans20pt7b.h"
//#include "FreeSans18pt7b.h"
//#include "FreeSans12pt7b.h"
//#include "FreeSans9pt7b.h"
//#include "FreeSans7pt7b.h"
#include "Orkney20pt7b.h"
#include "OrkneyRegular20pt7b.h"
//#include "Orkney8pt7b.h"
#include "OrkneyRegular9pt7b.h"
#include "OrkneyMedium9pt7b.h"

using namespace std;

int main (void)
{
  Display_SH1106 display;
  display.init();
  display.sendCommand(SH1106_DISPLAYON, SH1106_INVERTDISPLAY);
  display.sendCommand(SH1106_COMSCANINC, SH1106_NOP);
  display.clearDisplay();
  
  GFXfont font = OrkneyRegular20pt7b;
  display.setFont(font);

  display.displayTime();

  display.setCursor(0,61);
  font = OrkneyMedium9pt7b;
  display.setFont(font);

  string test="odoo";
  display.displayString(test);

  display.fillFullScreen();
  //display.waitForReturnKey();
  

  //display.sendCommand(SH1106_NORMALDISPLAY, SH1106_COMSCANDEC);
  //display.sleep(1,100);
  //display.sendCommand(SH1106_NORMALDISPLAY, SH1106_DISPLAYOFF);
  close(display.getFileDevice());// Close Driver = release I2C bus access
  return 0;
}