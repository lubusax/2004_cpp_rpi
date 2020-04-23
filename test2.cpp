#include <iostream>
#include "Temperatur.h"

using namespace std;

#define PROGRAMM_VERSION 2

int main()
{
	cout << "www.wenzlaff.de" << endl;
	cout << "Programm Version " << PROGRAMM_VERSION << endl;

	Temperatur temperatur;
	temperatur.ausgeben();

	return 0;
}