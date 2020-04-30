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

  _width = SH1106_WIDTH;  //128
  _height = SH1106_HEIGHT; //64

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
  str[0] = CONTROL_BYTE_LAST_COMMAND;
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
  int     i, j      {0};
  int     result, errsv {0};
  char    str[131]       {0};
  int    columnOffset  {0x02};

  int p = 0;

  for ( i = 0; i < 8; i++) {
    str[0] = 0x40; 
    for ( j = 0; j < 128; j++, p++) str[j+1] = _fullScreen[p];
    sendCommand(SH1106_SETPAGE + i, SH1106_NOP);
    sendCommand(0x10, columnOffset); //set column address
    result = write(_fileDevice,(char*)str,131);
    errsv = errno;
    if (result<0) {
      std::cerr << "Failed to write to i2c bus **********" << std::endl;
      std::cerr << "method Display_SH1106::fillFullScreen" << std::endl;
      std::cerr << strerror(errsv) << std::endl;
      return result; 
    }
  }  
  
  return 1;
}

// write the contents of some external file
// into the class variable _pfullScreen
int Display_SH1106::readFullScreen(const char * file) {

  std::fstream in_file {file, std::ios::in | std::ios::binary};

  if (!in_file) {
    std::cerr << "file open error " << std::endl;
    return -1; 
  }
  
  for ( int i = 0; i < 1024; i++) in_file.get(_fullScreen[i]);

  in_file.close();

  return 0;
}

// write the contents of the class variable _pfullScreen 
// into some external file
int Display_SH1106::writeFullScreen(const char * file) {

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

void Display_SH1106::writePixel(int16_t x, int16_t y, uint16_t color) {
  drawPixel(x, y, color);
  return;
}

void Display_SH1106::drawPixel(int16_t x, int16_t y, uint16_t color) {
  drawPixel(x,y);
  return;
}

void Display_SH1106::drawPixel(int16_t x, int16_t y) {
  if (x>=0 and x< _width and y>=0 and y<_height){
    int page = y>>3;
    int bitNumber = y & 0b111;
    int index = page * _width + x;
    _fullScreen[index] = _fullScreen[index] | (1<<bitNumber);
  }
  else {
    std::cerr << "Trying to draw a pixel out of limits*" << std::endl;
    return;
  }
  return;
}
void Display_SH1106::waitForReturnKey() {
  do { std::cout << '\n' << "Press return to continue...";
    } while (!std::cin.get());
  return;
}

void Display_SH1106::drawChar(int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y) {
  drawChar(x,y,c);
  return;
}

void Display_SH1106::drawChar(
      int16_t x, int16_t y, unsigned char c) {

  c = c - (uint8_t) _font.first;
  GFXglyph *glyph = (_font.glyph)+ c;
  uint8_t * bitmap = (_font.bitmap);
  uint16_t  bo    = (glyph->bitmapOffset);
  uint8_t   w     = (glyph->width),
            h     = (glyph->height);
  int8_t    xo    = (glyph->xOffset),
            yo    = (glyph->yOffset);
  uint8_t   xx, yy, bits = 0, bit = 0;

  //printf("font first: %d", c);

  // Todo: Add character clipping here

  for (yy = 0; yy < h; yy++) {
    for (xx = 0; xx < w; xx++) {
      if (!(bit++ & 7)) {
        bits = (bitmap[bo++]);
      }
      if (bits & 0x80) {
          drawPixel(x + xo + xx, y + yo + yy);
      }
      bits <<= 1;
    }
  }
  return;
}

void Display_SH1106::setFont(const GFXfont f) {
  
  _font = f;
  
  return;
}