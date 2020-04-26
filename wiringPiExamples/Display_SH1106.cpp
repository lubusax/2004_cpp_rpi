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

#define sh1106_swap(a, b) { int16_t t = a; a = b; b = t; }

Display_SH1106::Display_SH1106(){}
Display_SH1106::~Display_SH1106(){}

int Display_SH1106::init() {
  const char *  device    = "/dev/i2c-1" ;
  int           result, errsv = 0;

  // opening this file open, we open the bus
    // (acquire I2C bus access) for reading and writing.
  if ((_fileDevice = open(device, O_RDWR)) >= 0) {
    // After successfully acquiring bus access,
      // ioctl initiate communication with the peripheral.
      // It does this by sending out the seven bit
      // I2C address of the device followed by a read/write bit.
      // The bit is set to 0 for writes and 1 for reads.
      // The calls to read and write after the ioctl will automatically
      // set the proper read and write bit when signaling the peripheral. 
    if (ioctl(_fileDevice, I2C_SLAVE, SH1106_I2C_ADDRESS) < 0) {
      errsv = errno;
      result =-1;
      printf(
        "Unable to initiate communication with the I2C device: %s\n",
        strerror(errsv));
    }
  }
  else{
    result =-1;
    errsv = errno;
    printf("Unable to acquire I2C bus access: %s\n", strerror(errsv));
  }
  return result;
}

int Display_SH1106::sendCommand(char c1, char c2) { 
  static char   str[3] = {0};
  char *        buf = str;
  str[0] = 0x00;
  str[1] = c1;
  str[2] = c2;
  int           result = write(_fileDevice,buf,3);
  int           errsv = errno;

  if (result<0) {
    printf(
      "Failed to write to the i2c bus commands\n sendCommand error string  %s\n",
      strerror(errsv));
    printf("error number: %d \n",errsv);
  }
  return result;
}

int Display_SH1106::clearDisplay(){
  char        i, j;
  int         result, errsv;
  char        str[17] = {0}; //sh1106 moves to the next column 16 times
  char *      buf = str;
  str[0]     = 0x40;
  char columnOffset = 0x02;
  //sh1106 has 132 columns but the display shows only 128
  //this is why there is an offset of 2 columns

  for ( i = 0; i < 8; i++) {
    sendCommand(SH1106_SETPAGE + i, SH1106_NOP);
    for ( j = 0; j < 8; j++) {
      sendCommand(0x10+j, columnOffset); //set column address
      result = write(_fileDevice,buf,17);
      errsv = errno;
      if (result<0) {
        printf(
          "Failed to write to the i2c bus commands (clear Display) %s\n", strerror(errsv));
        printf("error number: %d \n",errsv);
      }
    }
  }  
  
  return result;
}

int Display_SH1106::fileDevice() {
  return _fileDevice;
}

int Display_SH1106::fullScreen(char * pPicture){
  char      i, j, k;
  int       result, errsv;
  char        str[17];
  char *      pStr = str;
  char columnOffset = 0x02;

  int p = 0;

  for ( i = 0; i < 8; i++) {
    sendCommand(SH1106_SETPAGE + i, SH1106_NOP);
    for ( j = 0; j < 8; j++) {
      str[0] = 0x40;       
      for ( k = 0; k < 16; k++, p++) {
        str[k+1] = pPicture[p];
        //printf("char: %d %d\n",p, buf2[k+1]);
      }
      sendCommand(0x10+j, columnOffset); //set column address
      result = write(_fileDevice,pStr,17);
      errsv = errno;
      if (result<0) {
        printf(
          "Failed to write to the i2c bus commands (clear Display) %s\n", strerror(errsv));
        printf("error number: %d \n",errsv);
      }
    }
  }  
  
  return result;
}
