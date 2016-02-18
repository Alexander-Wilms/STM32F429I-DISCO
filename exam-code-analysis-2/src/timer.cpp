// g++ -std=c++0x main.cpp


// keinen iostream oder stdio.h verwenden, da Funktionen verwqendet werden
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

#include "timer.h"

Timer::Timer(int stu_input, int min_input, int sec_input, int hun_input)
{
	stu = stu_input%100;
	min = min_input%60;
	sec = sec_input%60;
	hun = ((hun_input%100)/10)*10;
}

int Timer::getStu()
{
   return stu;
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

void Timer::setStu(int min_input)
{
   stu = min_input%60;
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
   	stringstream output;
	output << setfill('0') << setw(2) << stu << ":" <<  std::setfill('0') << setw(2) << min << ":" << setfill('0') << setw(2) << sec;
	string returnvalue = output.str();
	return returnvalue;
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
