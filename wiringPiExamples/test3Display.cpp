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

int main (void)
{
  Display_SH1106 display;
  display.init();
  display.sendCommand(SH1106_DISPLAYON, SH1106_NORMALDISPLAY);
  display.sendCommand(SH1106_COMSCANINC, SH1106_NOP);
  display.clearDisplay();

  //char originalArray[] = LOGO_ADAFRUIT;
  
  // int size = sizeof charArray/ sizeof charArray[0];
  //std::cout << size << std::endl;
  
  //display.setFullScreen(originalArray);
  //display.writeFullScreen("testOutFile");
  display.readFullScreen("testOutFile");
  display.fillFullScreen();
  do { std::cout << '\n' << "Press return to continue...";
  } while (!std::cin.get());
  //display.clearDisplay();
  //display.sleep(4,100);
  display.sendCommand(SH1106_INVERTDISPLAY, SH1106_COMSCANDEC);
  display.sleep(1,100);
  display.sendCommand(SH1106_NORMALDISPLAY, SH1106_DISPLAYOFF);
  close(display.getFileDevice());// Close Driver = release I2C bus access
  return 0;
}