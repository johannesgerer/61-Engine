#pragma once

const float PI=3.14159265359F;

//Signum
float sgn(float x)
{
	float y=1;

if(x<0)
	y=-1;

return y;
}

float betrag(float x)
{
	if(x<0)
		x=-x;
	return x;
}
		

#include <math.h>