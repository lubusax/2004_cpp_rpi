#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/syscall.h>

#define I2C_SLAVE	0x0703

int main (void)
{
  //const int     devID     = 0x3C;
  const char *  device    = "/dev/i2c-1" ;
  int           fileDevice ;
  int           result;
  int           errsv;
  //int           addr = 0b00111010;
  int           addr = 0x3C;

  fileDevice = open(device, O_RDWR);
  errsv = errno;

  if (fileDevice < 0) {
    printf("Unable to open I2C device: %s\n", strerror(errsv));
  }
  
  result = ioctl(fileDevice, I2C_SLAVE, addr);
  errsv = errno;

  if (result < 0) {
    printf("Unable to select I2C device: %s\n", strerror(errsv));
  }

  //unsigned char reg = 0x10; // Device register to access
  //buf[0] = reg;
  char str[10];
  char *buf = str;

  buf[0] = 0xA5;
  if (write(fileDevice,buf,1) != 1) {
      errsv = errno;
      printf("Failed to write to the i2c bus %s\n", strerror(errsv));
      printf("\n\n");
  }

  return 0;
}