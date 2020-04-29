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
#include <fstream>
#include <iostream>

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

int Display_SH1106::sendCommand(const char c1, const char c2) { 
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
  for ( int i = 0; i < 1024; i++) _fullScreen[i]= 0;
  return fillFullScreen();
}

int Display_SH1106::getFileDevice() {
  return _fileDevice;
}

// 

int Display_SH1106::fillFullScreen(){
  int     i, j, k       {0};
  int     result, errsv {0};
  char    str[17]       {0};
  // char *  pStr          =str;
  int    columnOffset  {0x02};

  int p = 0;

  for ( i = 0; i < 8; i++) {
    sendCommand(SH1106_SETPAGE + i, SH1106_NOP);
    for ( j = 0; j < 8; j++) {
      str[0] = 0x40;       
      for ( k = 0; k < 16; k++, p++) str[k+1] = _fullScreen[p];
      sendCommand(0x10+j, columnOffset); //set column address
      result = write(_fileDevice,(char*)str,17);
      errsv = errno;
      if (result<0) {
        std::cerr << "Failed to write to i2c bus **********" << std::endl;
        std::cerr << "method Display_SH1106::fillFullScreen" << std::endl;
        std::cerr << strerror(errsv) << std::endl;
        return result; 
      }
    }
  }  
  
  return 1;
}

// write the contents of some external file
// into the class variable _pfullScreen
int Display_SH1106::readFullScreen(char const * const file) {

  char charArray[1024];
  std::fstream in_file {file, std::ios::in | std::ios::binary};

  if (!in_file) {
    std::cerr << "file open error " << std::endl;
    return -1; 
  }
  
  for ( int i = 0; i < 1024; i++) in_file.get(charArray[i]);

  in_file.close();

  _pFullScreen = charArray;
  return 0;
}

// write the contents of the class variable _pfullScreen 
// into some external file
int Display_SH1106::writeFullScreen(char const * const file) {

  std::fstream out_file {file, std::ios::out | std::ios::binary};

  if (!out_file) {
    std::cerr << "file open error " << std::endl;
    return -1; 
  }

  for (int i = 0; i < 1024; i++) out_file.put(_pFullScreen[i]);

  out_file.close();

  return 0;
}

// write the contents of the passed function variable pfullScreen 
// into the class variable _pfullScreen
int Display_SH1106::setFullScreen(char const * const pFullScreen){
  for ( int i = 0; i < 1024; i++) _fullScreen[i]= pFullScreen[i];
  return 0;
}

char * Display_SH1106::getFullScreen() {
  return _pFullScreen;
}

int Display_SH1106::sleep(int seconds, int milliseconds){
  if (seconds<0) seconds=0;
  if (seconds>60) seconds =60;
  if (milliseconds<0) milliseconds=0;
  if (milliseconds>999) milliseconds =999;
  if (seconds==0 and milliseconds==0) return -1;
  long nanoseconds = (long) (milliseconds*1000000);
  struct timespec req, rem;
  req.tv_sec = seconds;
  req.tv_nsec = nanoseconds;
  return nanosleep(&req , &rem);
}
