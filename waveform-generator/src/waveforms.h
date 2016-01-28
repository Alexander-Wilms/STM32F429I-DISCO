#define PI 3.14159265
#define WIDTH 320
#define HEIGHT 240
#define NmbrOfPeriods 2
#include <math.h>

void sine(int * x, int * y)
{
	for(int i = 0;i<WIDTH;i++)
	{
		x[i]=i;
		y[i]=sin(i*2*PI/WIDTH*NmbrOfPeriods)*HEIGHT/2;
	}
}

void square(int * x, int * y)
{
	bool high = 1;
	for(int i = 0;i<WIDTH;i++)
	{
		x[i]=i;
		if(high)
			y[i]=HEIGHT-50;
		else
			y[i]=50;
		if(i%(WIDTH/2/3)==0)
				high = !high;
	}
}

void sawtooth(int * x, int * y)
{
	bool high = 1;
	float j = 0;
	for(int i = 0;i<WIDTH;i++)
	{
		x[i]=i;
		if(high)
		{
			y[i]=(0.4*HEIGHT)*j+HEIGHT/2;
			j += 1/(0.4*HEIGHT);
		}
		else
			{
			y[i]=0+HEIGHT/2;
			j=0;
			high = !high;
		}
		if(i!=0 && i%(WIDTH/2/NmbrOfPeriods)==0)
			high = !high;
	}
}

void triangle(int * x, int * y)
{
	bool high = 1;
	float j = 0;
	for(int i = 0;i<WIDTH;i++)
	{
		x[i]=i;
		if(high)
		{
			y[i]=(0.4*HEIGHT)*j+HEIGHT/2;
			j += 1/(0.4*HEIGHT);
		}else
		{
			y[i]=(0.4*HEIGHT)*j+HEIGHT/2;
			j -= 1/(0.4*HEIGHT);
		}
		if(i!=0 && i%(WIDTH/2/1)==0)
			high = !high;
	}
}
