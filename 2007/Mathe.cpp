#include "Mathe.h"

//Signum
double sgn(double x)
{
	double y=0;

	if(x<0)
		y=-1;
	if(x>0)
		y=1;

	return y;
}

//Betrag
double betrag(double x)
{
	if(x<0)
		x=-x;
	return x;
}

double Sinus(double x)
{
	if(betrag(x) ==90)
		return 1*sgn(x);
	if(x==0 || betrag(x)==180)
		return 0;

	return sin(x/180*PI);
}

double Cosinus(double x)
{
	
	return Sinus(90-x);
}

//Potenzrechnung
double sq(double x)	//Hoch 2 (sq [=square] im Gegensatz zu sqrt [=sqare root])
{
	return x*x;
}

double hoch(double x,int y)	//"x hoch y"
{
	double ausgabe=1;
	for(int i=0;i<betrag(y);i++)
		if(y>0)
			ausgabe*=x;
		else
			ausgabe/=x;
	return ausgabe;
}