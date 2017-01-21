//################################
//Vektor.h (letzte Änderung: 26.10.03 20:48)
//################################
/*
Diese Datei enthält

a) Klasse CVektor. 
---------------------------------------
-Zur Darstellung einer Position im 3D-Raum mit einer x-, y- und z- Koordinate dar.
-Zur Darstellung einer Orientierung im 3D-Raum, in dem der Vektor die Richtung 
	und sein Betrag den Drehwinkel um die Drehachse(Vektor) angeben
-Außerdem werden folgende Vektor-Operatoren definiert:
	+Addition/Subtraktion	(Vektor +/- Vektor	 = Vektor)
	+Skalarmulitplikation	(Vektor * Zahl(float)= Vektor)
	+Skalardivision			(Vektor / Zahl(float)= Vektor)
	+Skalarprodukt			(Vektor * Vektor	 = Zahl)
	+Vektorprodukt			(Vektor ^ Vektor	 = Vektor)
	+Betrag					(!Vektor			 = Zahl)
	+Einheitsvektor			(~Vektor			 = Vektor)

b) VektorZeichnen(farbe, v_start, v)
---------------------------------------------
*/

#pragma once

#include "GLInclude.h"
#include "Mathe.h"


class CVektor
{
public:
//Membervariablen:
	float		x,y,z;						//3 Koordinaten

//Konstruktoren:
	CVektor (){x=0;y=0;z=0;};							//Standardkonstruktor & 
	CVektor (float ix, float iy, float iz)	//Konstruktor mit direkter Dateneingabe
	{x=ix;y=iy;z=iz;};

//Operatoren:
	void operator +=(CVektor& u);	//Addition
	void operator -=(CVektor& u);	//Subtraktion
	void operator *=(float &s);		//Skalarmultiplikation
	void operator /=(float &s);		//Skalardivision
	void operator ^=(CVektor &u);	//Vektorprodukt
};

CVektor	operator +(CVektor& v,CVektor& u)	//Addition
{
	return CVektor (u.x + v.x,
					u.y + v.y,
					u.z + v.z);
}

CVektor	operator -(CVektor& u,CVektor& v)	//Subtraktion
{
	return CVektor (u.x - v.x,
					u.y - v.y,
					u.z - v.z);
}

CVektor operator *(CVektor& v, float s)		//Skalarmultiplikation
{
	return CVektor(v.x*s, v.y*s, v.z*s);
}

CVektor operator /(CVektor& v,float s)		//Skalardivision
{
	return v*(1/s);
}

float	operator *(CVektor& v,CVektor& u)	//Skalarprodukt
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}

CVektor operator ^(CVektor &u,CVektor &v)	//Vektorprodukt
{
	return CVektor (u.y*v.z - u.z*v.y,
					u.z*v.x - u.x*v.z,
					u.x*v.y - u.y*v.x);
}

float operator !(CVektor &v)				//Betrag
{
	return sqrtf(v.x * v.x+
				v.y * v.y+
				v.z * v.z);
}

CVektor operator~(CVektor& v)				//Einheitsvektor
{	
	float betrag=!v;
	if (betrag>0)
		return v*(1/!v);
	else
		return CVektor(0,0,0); 
}

void CVektor::operator +=(CVektor& u){*this=*this+u;	return;}	//Addition
void CVektor::operator -=(CVektor& u){*this=*this-u;	return;}	//Subtraktion
void CVektor::operator *=(float &s)	{*this=*this*s;		return;}	//Skalarmultiplikation
void CVektor::operator /=(float &s)	{*this=*this*(1/s);	return;}	//Skalardivision
void CVektor::operator ^=(CVektor &u){*this=*this^u;	return;}	//Vektorprodukt

void LinieZeichen(float *farbe, CVektor v_start, CVektor v)
{
glLineWidth(1.6F);
glBegin(GL_LINES);
	glColor4fv(farbe); glVertex3f(v_start.x,v_start.y,v_start.z);
	glColor4fv(farbe); glVertex3f(v.x,v.y,v.z);
	glEnd();
glLineWidth(1);
}

#include "Schrift.h"
extern CSchrift	g_Schrift;

void VektorZeichnen(float *farbe,CVektor v_start,CVektor v, bool VektorDifferenz, const char* fmt, ...)
{	
	char		text[256];								// Holds Our String
	//Start -- String umwandel
	va_list		ap;										// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--	

	if(!VektorDifferenz)//Entweder "v" soll gezeichnet werden
	{
		g_Schrift.Schreiben(farbe, v_start+v/2, text);
		LinieZeichen(farbe, v_start,v+v_start);
	}
	else
	{
		g_Schrift.Schreiben(farbe, (v_start+v)/2, text);
		LinieZeichen(farbe, v_start,v);
		v-=v_start;		//v um v_start verschieben
	}

	float x=!v;

	if(x>0)
	{
	glPushMatrix();

	glTranslatef(v_start.x,v_start.y,v_start.z);		//OpenGL-Raum verschieben um v_start	

	glRotatef(acosf(v.x/x)/PI*180,0,-v.z,v.y); //OpenGL-Raum um z-Achse drehen

	float a=0.7f;	//Pyramidenhöhe
	float b=0.3f;	//Pyramidenbreite/2
	float c=0.1f;	//Pfeildicke
		
	//#############Die Pfeilspitze
	glBegin(GL_TRIANGLES);
	glNormal3f( 1.0f, 1.0f, 0.0f);
	glColor4fv(farbe); glVertex3f(x,0,0);
	glColor4fv(farbe); glVertex3f(x-a,+b,+b);
	glColor4fv(farbe); glVertex3f(x-a,+b,-b);
	
	glNormal3f( 1.0f, -1.0f, 0.0f);
	glColor4fv(farbe); glVertex3f(x,0,0);
	glColor4fv(farbe); glVertex3f(x-a,-b,-b);
	glColor4fv(farbe); glVertex3f(x-a,-b,+b);

	glNormal3f( 1.0f, 0.0f, -1.0f);
	glColor4fv(farbe); glVertex3f(x,0,0);
	glColor4fv(farbe); glVertex3f(x-a,-b,-b);
	glColor4fv(farbe); glVertex3f(x-a,+b,-b);

	glNormal3f( 1.0f, 0.0f, 1.0f);
	glColor4fv(farbe); glVertex3f(x,0,0);
	glColor4fv(farbe); glVertex3f(x-a,b,+b);
	glColor4fv(farbe); glVertex3f(x-a,-b,+b);
	glEnd();
	//#############Die Pfeilspitze - Ende


	//#############Den Pfeilkörper
	
	glBegin(GL_QUADS);
	
	glNormal3f( 0.0f, 0.0f, -1.0f);
	glColor4fv(farbe); glVertex3f(x-a,b,b);
	glColor4fv(farbe); glVertex3f(x-a,-b,b);
	glColor4fv(farbe); glVertex3f(x-a,-b,-b);
	glColor4fv(farbe); glVertex3f(x-a,b,-b);

	if(x>a)// (nur wenn Gesamtlänge größer als Pfeilspitzenlänge)
	{
	glNormal3f( 0.0f, 0.0f, -1.0f);
	glColor4fv(farbe); glVertex3f(0, c,-c);
	glColor4fv(farbe); glVertex3f(0, c, c);
	glColor4fv(farbe); glVertex3f(0,-c, c);
	glColor4fv(farbe); glVertex3f(0,-c,-c);

	glNormal3f( 0.0f, 1.0f, 0.0f);
	glColor4fv(farbe); glVertex3f(x-a, c,-c);
	glColor4fv(farbe); glVertex3f(  0, c,-c);
	glColor4fv(farbe); glVertex3f(  0, c, c);
	glColor4fv(farbe); glVertex3f(x-a, c, c);

	glNormal3f( 0.0f, -1.0f, 0.0f);
	glColor4fv(farbe); glVertex3f(x-a, -c,-c);
	glColor4fv(farbe); glVertex3f(  0, -c,-c);
	glColor4fv(farbe); glVertex3f(  0, -c, c);
	glColor4fv(farbe); glVertex3f(x-a, -c, c);

	glNormal3f( 0.0f, 0.0f, 1.0f);
	glColor4fv(farbe); glVertex3f(x-a,  c, c);
	glColor4fv(farbe); glVertex3f(  0,  c, c);
	glColor4fv(farbe); glVertex3f(  0, -c, c);
	glColor4fv(farbe); glVertex3f(x-a, -c, c);

	glNormal3f( 0.0f, 0.0f, -1.0f);
	glColor4fv(farbe); glVertex3f(x-a,  c, -c);
	glColor4fv(farbe); glVertex3f(  0,  c, -c);
	glColor4fv(farbe); glVertex3f(  0, -c, -c);
	glColor4fv(farbe); glVertex3f(x-a, -c, -c);
	}
	glEnd();	
	// (nur wenn Gesamtlänge größer als Pfeilspitzenlänge)Den Pfeilkörper - Ende
	glPopMatrix();
	}

return;
}
