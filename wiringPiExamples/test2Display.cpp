// https://books.google.de/books?id=Udu6BQAAQBAJ&pg=PA286&lpg=PA286&dq=failed+to+write+to+file+i2c-1+in+c&source=bl&ots=C_agSxvDme&sig=ACfU3U1lqcrMXdWSzWtBclsGO2835vEC7g&hl=en&sa=X&ved=2ahUKEwiQ5I7m54HpAhUvSxUIHZgeDawQ6AEwAHoECAsQAQ#v=onepage&q=failed%20to%20write%20to%20file%20i2c-1%20in%20c&f=false
// Exploring BeagleBone: Tools and Techniques for Building with Embedded Linux
// By Derek Molloy
// page 286 I2C Communication in C

//https://elinux.org/Interfacing_with_I2C_Devices


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
#include "test2Display.h"

//#include <sys/syscall.h>

#define I2C_SLAVE	0x0703

int main (void)
{
  const char *  device    = "/dev/i2c-1" ;
  int           fileDevice ;
  int           result;
  int           errsv;

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

  char str[MAX_WRITE_BUFFER_SIZE];
  char *buf = str;

  buf[0] = 0x00;
    // always begin communication with 0x00
  buf[1] = SH1106_DISPLAYON;
  buf[2] = SH1106_DISPLAYALLON;

  result = write(fileDevice,buf,3);
    //result is the number of bytes writen in the file
    // if there is an error result is -1
  errsv = errno;
  // printf("result: %d \n",result);
  // printf("error number: %d \n",errsv);

  if (result<0) {
    printf("Failed to write to the i2c bus %s\n", strerror(errsv));
    printf("error number: %d \n",errsv);
  }

  nanosleep((const struct timespec[])
    {{  2           /* seconds */,
        500000000L  /* nanoseconds */}}, NULL);

  buf[0] = 0x00;
  buf[1] = SH1106_DISPLAYOFF;

  result = write(fileDevice,buf,2);
  errsv = errno;
  if (result < 0) {
    printf("Failed to write to the i2c bus %s\n", strerror(errsv));
    printf("error number: %d \n",errsv);
  }


  return 0;
}