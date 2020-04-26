// Exploring BeagleBone:
// Tools and Techniques for Building with Embedded Linux
// By Derek Molloy
// page 286 I2C Communication in C

//https://elinux.org/Interfacing_with_I2C_Devices

//https://embetronicx.com/tutorials/linux/device-drivers/ioctl-tutorial-in-linux/



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

  // char Luis_bits[] = {
  //  0xff, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20,
  //  0xf2, 0xff, 0xff, 0xff, 0xff, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x88, 0x47, 0xea, 0xff, 0xff, 0xff, 0xff, 0x17, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xe8, 0xff, 0xff, 0xff,
  //  0xff, 0x0f, 0x80, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4a,
  //  0xe9, 0xff, 0xff, 0xff, 0xff, 0x87, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x14, 0xf2, 0xff, 0xff, 0xff, 0xff, 0x97, 0x84, 0x08,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0xff, 0xff, 0x7b,
  //  0xff, 0x4f, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14,
  //  0xea, 0xff, 0xff, 0x81, 0xff, 0x57, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x40, 0xe8, 0xff, 0x7f, 0x00, 0xff, 0x6f, 0x01, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa1, 0xff, 0x7f, 0x80,
  //  0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
  //  0xc8, 0xff, 0x7f, 0x58, 0xff, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x01, 0x00, 0x00, 0x04, 0xa2, 0xff, 0x3f, 0x14, 0xff, 0x1f, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x88, 0xff, 0xbf, 0x0a,
  //  0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xe4, 0xff, 0x15, 0x40,
  //  0xa0, 0xfe, 0x1f, 0x03, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0xe0, 0xfd, 0x07, 0x00, 0x80, 0xff, 0x3f, 0x0b, 0xff, 0x1f, 0x00, 0x00,
  //  0x10, 0x00, 0x00, 0x40, 0xe0, 0x7c, 0x2a, 0x00, 0x24, 0xff, 0xdf, 0x00,
  //  0xff, 0x2f, 0x00, 0xc0, 0x2f, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x14, 0x00,
  //  0x40, 0xfe, 0xaf, 0x05, 0xff, 0x1f, 0x00, 0xfc, 0x1f, 0x00, 0x00, 0x00,
  //  0xe0, 0x7f, 0x00, 0x00, 0x80, 0xfe, 0x77, 0x00, 0xff, 0x2f, 0x00, 0xfd,
  //  0x3f, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x0a, 0x00, 0x10, 0xfd, 0x4f, 0x00,
  //  0xff, 0x1f, 0xc0, 0xfa, 0x3e, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00,
  //  0x40, 0xfa, 0x37, 0x00, 0xff, 0x2f, 0xa8, 0xf9, 0x3e, 0x00, 0x00, 0x00,
  //  0x80, 0x03, 0x00, 0x00, 0x80, 0xfe, 0x6d, 0x00, 0xe7, 0x8f, 0x30, 0xf8,
  //  0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xfa, 0x1b, 0x00,
  //  0xd7, 0x1f, 0x50, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x40, 0xfe, 0xb7, 0x00, 0xd7, 0x0f, 0x2a, 0xf0, 0x0f, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x0b, 0x00, 0x83, 0x3f, 0x00, 0xe0,
  //  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xf9, 0xbb, 0x00,
  //  0x07, 0x0f, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x80, 0xfe, 0x02, 0x00, 0x07, 0x1e, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x10, 0xf8, 0x05, 0x00, 0x0f, 0x0d, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x7d, 0x00, 0x00,
  //  0x0f, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x02, 0x75, 0x00, 0x80, 0x0f, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x50, 0x5d, 0x00, 0x80, 0x0f, 0x30, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xb4, 0x00, 0x80,
  //  0x1f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x28, 0x6d, 0x00, 0x80, 0x3f, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x40, 0xa9, 0x04, 0xa0, 0x7f, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x2d, 0x0a, 0xc0,
  //  0x7f, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x40, 0x6a, 0x02, 0xc0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x94, 0x95, 0x04, 0x80, 0xff, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0xaa, 0x00, 0xc9,
  //  0xff, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0xa0, 0x4a, 0x00, 0xc0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0xa4, 0x52, 0x00, 0xc2, 0xff, 0x03, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x15, 0x00, 0xc0,
  //  0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x10, 0x29, 0x00, 0xe0, 0xff, 0x83, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x25, 0x2a, 0x00, 0xe0, 0xff, 0x07, 0x00, 0x00,
  //  0x00, 0x00, 0xa0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x12, 0x00, 0xf0,
  //  0xff, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
  //  0x90, 0x2a, 0x00, 0xf0, 0xff, 0x07, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x52, 0x0a, 0x00, 0xf8, 0xff, 0x07, 0x00, 0x00,
  //  0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x31, 0x00, 0xfc,
  //  0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x24, 0x6a, 0x00, 0xff, 0xff, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x48, 0xf5, 0xf7, 0xff, 0xff, 0x07, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0xfa, 0xff, 0xff,
  //  0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0xa4, 0xfa, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x08, 0xfa, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0xf5, 0xff, 0xff,
  //  0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
  //  0x20, 0xfd, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x94, 0xfe, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xfc, 0xff, 0xff,
  //  0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x28, 0xfd, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0xa0, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0xfe, 0xff, 0xff,
  //  0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x20, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0xc9, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00,
  //  0x00, 0x80, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x90, 0xbd, 0xfe, 0xff,
  //  0xff, 0xff, 0x07, 0x00, 0x00, 0x20, 0xad, 0x0d, 0x00, 0x00, 0x00, 0x00,
  //  0x40, 0xdf, 0xfd, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0xa4, 0xed, 0x5b,
  //  0x00, 0x00, 0x00, 0x00, 0x54, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00,
  //  0x00, 0xd0, 0xbb, 0xb6, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xef, 0xff, 0xff,
  //  0xff, 0xff, 0x0f, 0x00, 0x00, 0xf0, 0xa6, 0xa2, 0x02, 0x00, 0x00, 0x00,
  //  0x48, 0x57, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xaa, 0x10, 0x40,
  //  0x00, 0x00, 0x00, 0x00, 0xd2, 0xff, 0xfd, 0xff, 0xff, 0xff, 0x1f, 0x00,
  //  0x00, 0x14, 0x05, 0x84, 0x00, 0x00, 0x00, 0x00, 0x60, 0xf7, 0xff, 0xff,
  //  0xff, 0xff, 0x1f, 0x00, 0x00, 0x11, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0xdd, 0xdf, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0xf0, 0x5d, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00,
  //  0x00, 0x80, 0x24, 0x80, 0x04, 0x00, 0x00, 0x00, 0xea, 0xff, 0xff, 0xff,
  //  0xff, 0xff, 0x3f, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0xf8, 0x7b, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x02, 0x00, 0x80,
  //  0x00, 0x00, 0x00, 0x40, 0xfe, 0xfe, 0xef, 0xff, 0xff, 0xff, 0x7f, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xfc, 0xed, 0xc3, 0xff,
  //  0xff, 0xff, 0x7f, 0x00, 0x00, 0x14, 0x00, 0x10, 0x00, 0x00, 0x00, 0x80,
  //  0xbe, 0xf7, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0x00, 0xfc, 0xef, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01,
  //  0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x80, 0xbf, 0x36, 0x00, 0xfe,
  //  0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x90,
  //  0xff, 0x3f, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00,
  //  0x00, 0x00, 0x00, 0xc0, 0xbf, 0x0c, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x07,
  //  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xfb, 0x0f, 0x00, 0xfe
  // };

  char        str[3000] = LOGO_ADAFRUIT;
  char *      pStr = str;

  display.fullScreen(pStr);

  nanosleep((const struct timespec[])
    {{  30          /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);
  //

  display.sendCommand(SH1106_DISPLAYOFF, SH1106_NOP);
  
  // printf("Close Driver, ie release I2C bus access\n");
  close(display.fileDevice());

  return 0;
}