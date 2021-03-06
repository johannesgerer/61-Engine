#ifndef Mathe
#define Mathe

//Einige mathematische Funktionen


#include <math.h>
const double PI=3.14159265359;

double sgn(double x);				//Signum
double betrag(double x);			//Betrag
double sq(double x);				//Quadrat
double hoch(double x,int y);		//x hoch y

double Sinus(double x);
double Cosinus(double x);

#include "Vektor.h"
#include "Matrix.h"
#include "Determinanten.h"

const double LE_potenz=7;
const double LE=hoch(10,-LE_potenz);

const double G=6.673e-11*hoch(LE,3);

#endif