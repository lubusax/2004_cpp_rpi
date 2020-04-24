#include <wiringPi.h>
#include <stdio.h>
// using namespace std;

int main (void)
{
  wiringPiSetup () ;
  pinMode (0, OUTPUT) ;
  printf ("Raspberry Pi blink\n") ;
  for (;;)
  {
    digitalWrite (0, HIGH) ; delay (500) ;
    digitalWrite (0,  LOW) ; delay (500) ;
  }
  return 0 ;
}