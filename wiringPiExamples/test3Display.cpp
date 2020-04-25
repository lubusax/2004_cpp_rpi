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
#include "test3Display.h"
#include <linux/i2c.h>     // needed to define I2C_SLAVE (0x0703)
#include <linux/i2c-dev.h> // needed to define I2C_SLAVE (0x0703)

int main (void)
{
  const char *  device    = "/dev/i2c-1" ;
  int           fileDevice, result, errsv, len ;

  // opening this file open, we open the bus
    // (acquire I2C bus access) for reading and writing.
  if ((fileDevice = open(device, O_RDWR)) < 0) {
    errsv = errno;
    printf("Unable to acquire I2C bus access: %s\n", strerror(errsv));
  }

  // After successfully acquiring bus access,
    // ioctl initiate communication with the peripheral.
    // It does this by sending out the seven bit
    // I2C address of the device followed by a read/write bit.
    // The bit is set to 0 for writes and 1 for reads.
    // The calls to read and write after the ioctl will automatically
    // set the proper read and write bit when signaling the peripheral. 
  if (ioctl(fileDevice, I2C_SLAVE, SH1106_I2C_ADDRESS) < 0) {
    errsv = errno;
    printf(
      "Unable to initiate communication with the I2C device: %s\n",
      strerror(errsv));
  }

  { // switch on, all display on
    static char   str[MAX_WRITE_BUFFER_SIZE] = COMMAND_ALLON;
    char *        buf = str;
    len  = LENGTH_ALLON;
    result = write(fileDevice,buf,len);
      //result is the number of bytes writen in the file
      // if there is an error result is -1
    errsv = errno;
    // printf("result: %d \n",result);
    // printf("error number: %d \n",errsv);

    if (result<0) {
      printf("Failed to write to the i2c bus %s\n", strerror(errsv));
      printf("error number: %d \n",errsv);
    }
  }

  nanosleep((const struct timespec[])
    {{  2           /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);
  //

  { // switch off
    static char  str[MAX_WRITE_BUFFER_SIZE] = 
      {INIT_COMMS, SH1106_DISPLAYOFF};
    char *        buf = str;
    len  = 2;
    result = write(fileDevice,buf,len);
    errsv = errno;
    if (result<0) {
      printf("Failed to write to the i2c bus %s\n", strerror(errsv));
      printf("error number: %d \n",errsv);
    }
  }

  // printf("Closing Driver, ie release I2C bus access\n");
  close(fileDevice);

  return 0;
}