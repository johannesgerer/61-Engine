//################################
//Vektor.h (letzte Änderung: 2.4.04 19:50)
//################################
/*
Diese Datei enthält

a) Klasse CVektor. 
---------------------------------------
-Zur Darstellung einer Position oder Richtung im 3D-Raum mit 3 + homogener Koordinate
-Membervariablen:
	+ double	x, y, z, w							//3 kartesische Koordinaten + Homogene Koordinate
-Konstruktoren:
	+ CVektor()								//Standardkonstruktor (0,0,0,1)
	+ CVektor(double ix, iy, iz, iw=1)		//Konstruktor für mit Parametern
-Operatoren:
	+ double operator () (int i);		//Gibt eine best. Koordinate aus 
	+ Addition/Subtraktion	(Vektor +/- Vektor	 = Vektor)
	+ Skalarmulitplikation	(Vektor * Zahl(double)= Vektor)
	+ Skalardivision		(Vektor / Zahl(double)= Vektor)
	+ Skalarprodukt			(Vektor * Vektor	 = Zahl)
	+ Vektorprodukt			(Vektor ^ Vektor	 = Vektor)
	+ Betrag				(!Vektor			 = Zahl)	//(ohne homogene Kooridnate)
	+ Einheitsvektor		(~Vektor			 = Vektor)
-Memberfunktionen:
	+Schreiben(bool rechts, int Zeile)

b) VektorZeichnen(int farbe,CVektor v_start,CVektor v, bool VektorDifferenz, const char* fmt, ...)
*/

#pragma once

class CMat;
 
class CVektor
{
public:
//Membervariablen:
	double		x,y,z,w;				//3 kartesische Koordinaten + homogene Koordinate
	double*	koordinaten_double;
	float*	koordinaten_float;
	bool K;

//Konstruktoren:
	CVektor (){x=0;y=0;z=0;w=1;K=false;};	//Standardkonstruktor (0,0,0,1)
	CVektor (double* f){x=*f++;y=*f++;z=*f++;w=*f++;K=false;};
	CVektor (double l, double i_w=1){x=l;y=l;z=l;w=i_w;K=false;};				//Standardkonstruktor (x,x,x,x)
	CVektor (int l, int i_w=1){x=(double)l;y=(double)l;z=(double)l;w=(double)i_w;K=false;};	
	CVektor (double ix, double iy, double iz, double iw=0)	//Konstruktor für mit Parametern
	{x=ix;y=iy;z=iz;w=iw;K=false;};


//Member-Funktionen:
	void Schreiben(bool rechts, int Zeile, bool iw=0);
	void Koordinate(int i,double f);	//Setzt den Wert einer best. Koordinate 

//Operatoren:
	double operator () (int i);			//Gibt eine best. Koordinate aus 

	CVektor	operator +(CVektor& v);		//Addition
	CVektor	operator -(CVektor& v);		//Subtraktion
	CVektor operator *(double s);		//Skalarmultiplikation
	CVektor operator /(double s);		//Skalardivision
	double	operator *(CVektor& u);		//Skalarprodukt
	CVektor operator ^(CVektor &v);		//Vektorprodukt
	double	operator !();				//Betrag
	double sqBetrag();
	CVektor operator ~();				//Einheitsvektor

	void operator +=(CVektor& u);		//Addition
	void operator -=(CVektor& u);		//Subtraktion
	void operator /=(double s);			//Skalardivision
	void operator *=(double s);			//Skalarmultiplikation
	void operator ^=(CVektor &u);		//Vektorprodukt	
	void operator *=(CMat M);			//Matrix
	bool operator ==(CVektor v);
};

void VektorZeichnen(int farbe,CVektor v_start,CVektor v, bool VektorDifferenz, const char* fmt, ...);



