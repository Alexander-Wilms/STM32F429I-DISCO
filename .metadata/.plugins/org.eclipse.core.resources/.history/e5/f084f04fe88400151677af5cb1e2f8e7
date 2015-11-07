// g++ -std=c++0x main.cpp


// keinen iostream oder stdio.h verwenden, da Funktionen verwqendet werden
#include <string>
#include <sstream>
using namespace std;

#include "timer.h"

Timer::Timer(int min_input, int sec_input, int hun_input)
{
	min = min_input%60;
	sec = sec_input%60;
	hun = ((hun_input%100)/10)*10;
}

int Timer::getMin()
{
   return min;
}

int Timer::getSec()
{
   return sec;
}

int Timer::getHun()
{
   return hun;
}

void Timer::setMin(int min_input)
{
   min = min_input%60;
}

void Timer::setSec(int sec_input)
{
   sec = sec_input%60;
}

void Timer::setHun(int hun_input)
{
   hun = ((hun_input%100)/10)*10;
}

std::string Timer::printtime()
{
   	std::stringstream output;
	output << min << ":" << sec << ":" << hun;
	string returnvalue = output.str();
	return returnvalue;
	return "";
}

/*int main()
{
	int min_input, sec_input, hun_input;
	cin >> min_input;
	cin >> sec_input;
	cin >> hun_input;
	Time timeobject(min_input,sec_input,hun_input);
	cout << timeobject.print() << endl;
	return 0;
}*/
