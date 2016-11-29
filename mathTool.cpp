//mathTool.cpp

#include <math.h>

//#include "cstlib.h"
#include "mathTool.h"
#define LOG_SMALL	((float)(-0.5E10))	
#define LOG_ZERO	((float)(-1.0E10))	
//#define MIN_FLOAT   ((float)(-1.0E20))  
//#define minLogExp	-23.0				
#define minLogExp	-10.0				


double LogAdd(double a,double b)
{
	double temp;
	double diff;
	double z;
   
	if (a<b)
	{
		temp = a; a = b; b = temp;
	}
	diff = b-a;
//	double minLogExp = -log(-LZERO);	//minLogExp ~= -10
	if (diff<minLogExp) 
		return  (a<LOG_SMALL)?double(LOG_ZERO):a;
	else 
	{
		z = exp(diff);
		return double(a+log(1.0+z));
	}
}



float LogAdd_Fast(float a,float b)
{
	float temp;
	float diff;

   
	if (a<b)
	{
		temp = a; a = b; b = temp;
	}
	diff = a-b;

	if(diff>-minLogExp)
	{
		return  (a<LOG_SMALL)?float(LOG_ZERO):a;
	}
	else
	{
//		double x1=diff;
//		double x2=x1*x1;
//		double x3=x2*x1;
//		double x4=x3*x1;
//		double x5=x4*x1;
		float y;
//		y=0.69419-0.51445*x1+0.15658*x2-0.023968*x3+0.0018205*x4-5.4482e-5*x5;

		y=(float)(0.69419+diff*(-0.51445+diff*(0.15658+diff*(-0.023968+diff*(0.0018205-diff*5.4482e-5)))));


		return (float) (a+y);
	}
}
