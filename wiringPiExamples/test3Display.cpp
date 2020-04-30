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
#include "FreeSansBold12pt7b.h"
#include "FreeSansBold18pt7b.h"
#include "FreeSansBold24pt7b.h"
#include "FreeSans24pt7b.h"

using namespace std;

int main (void)
{
  Display_SH1106 display;
  display.init();
  display.sendCommand(SH1106_DISPLAYON, SH1106_INVERTDISPLAY);
  display.sendCommand(SH1106_COMSCANINC, SH1106_NOP);
  display.clearDisplay();
  
  const GFXfont font = FreeSans24pt7b;
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  int hour = timeinfo->tm_hour;
  int min = timeinfo->tm_min;
  string clock= to_string(hour)+":"+to_string(min);
  
  
  display.setFont(font);

  int widthString = display.widthString();
  
  display.setCursor(10,40);
  unsigned char c = 0x38;
  display.drawChar(c);
  c--;
  display.drawChar(c);
  c--;
  display.drawChar(c);
  c--;
  display.drawChar(c);
  //char originalArray[] = LOGO_ADAFRUIT;
  
  // int size = sizeof charArray/ sizeof charArray[0];
  //cout << size << endl;
  
  //display.setFullScreen(originalArray);
  //display.writeFullScreen("testOutFile");
  //for (int i=0; i < 64; i++) display.drawPixel(i,i);

  display.fillFullScreen();
  display.waitForReturnKey();
  
  //display.clearDisplay();
  //display.sleep(4,100);
  display.sendCommand(SH1106_NORMALDISPLAY, SH1106_COMSCANDEC);
  display.sleep(1,100);
  display.sendCommand(SH1106_NORMALDISPLAY, SH1106_DISPLAYOFF);
  close(display.getFileDevice());// Close Driver = release I2C bus access
  return 0;
}