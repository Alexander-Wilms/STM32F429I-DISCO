// do not include iostream or stdio.h, since they use function calls
// that are not supported on the stm32

#include <string>
#include <sstream>
#include <iomanip>
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
	output << std::setfill('0') << std::setw(2) << min << ":" <<  std::setfill('0') << std::setw(2) << sec << ":" << std::setfill('0') << std::setw(2) << hun;
	string returnvalue = output.str();
	return returnvalue;
	return "";
}
