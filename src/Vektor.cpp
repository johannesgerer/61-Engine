#include "Mathe.h"
#include "Schrift.h"
#include "Farben.h"
#include "MatrizenStapel.h"
#include "Transformationen.h" 
#include "Primitive.h"

extern CFarben Farben;
extern CSchrift	g_Schrift;
extern CMatrizenStapel MatStapel;

double CVektor::sqBetrag()
{
	return	x * x+
			y * y+
			z * z;	
}


void CVektor::Koordinate(int i,double f)
{
	switch(i)
	{
	case 0: x=f; break;
	case 1: y=f; break;
	case 2: z=f; break;
	case 3: w=f; break;
	}
	return;
}

bool CVektor::operator ==(CVektor v)
{
	return (x==v.x && y==v.y && z==v.z && w==v.w);
}

double CVektor::operator () (int i)			//Gibt eine Koordinate aus (0->x, 1->y, 2->z, 3->w)
{
	switch(i)
	{
	case 0: return x;
	case 1: return y;
	case 2: return z;
	case 3: return w;
	}
	return 0;
}

CVektor	CVektor::operator +(CVektor& v)	//Addition
{
	return CVektor (x + v.x,
					y + v.y,
					z + v.z,
					w + v.w);
}

CVektor	CVektor::operator -(CVektor& v)	//Subtraktion
{
	return CVektor (x - v.x,
					y - v.y,
					z - v.z,
					w - v.w);
}

CVektor CVektor::operator *(double s)		//Skalarmultiplikation
{
	return CVektor(x*s, y*s, z*s, w);
}

CVektor CVektor::operator /(double s)		//Skalardivision
{
	return *this*(1/s);
}

double	CVektor::operator *(CVektor& v)	//Skalarprodukt
{
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

CVektor CVektor::operator ^(CVektor &v)	//Vektorprodukt
{
	return CVektor (y*v.z - z*v.y,
					z*v.x - x*v.z,
					x*v.y - y*v.x,
					0);
}

double CVektor::operator !()				//Betrag
{
	return sqrt(x * x+
				y * y+
				z * z);
}

CVektor CVektor::operator~()				//Einheitsvektor
{	
	double betrag=!*this;
	if (betrag>0)
		return *this*(1/betrag);
	else
		return CVektor(0,0,0); 
}


void CVektor::operator +=(CVektor& u){*this=*this+u;	return;}	//Addition
void CVektor::operator -=(CVektor& u){*this=*this-u;	return;}	//Subtraktion
void CVektor::operator /=(double s)	 {*this=*this*(1/s);	return;}	//Skalardivision
void CVektor::operator *=(double s)	 {*this=*this*s;	return;}	//Skalarmultiplikation
void CVektor::operator ^=(CVektor &u){*this=*this^u;	return;}	//Vektorprodukt
void CVektor::operator *=(CMat M)	 
{
	*this=M**this;
}	//Matrixtransformation

void VektorZeichnen(int farbe,CVektor v_start,CVektor v, bool VektorDifferenz, const char* fmt, ...)
{	
	double faktor=3;

	char		text[256];								// Holds Our String
	//Start -- String umwandel
	va_list		ap;										// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--	

	if(!VektorDifferenz)//Entweder "v" soll gezeichnet werden
	{
		CVektor t1=v_start+v/2;
		g_Schrift.Schreiben(farbe,t1 , text);
	}
	else//Oder Verbindungsvektor v_start->v
	{
		CVektor t1=(v_start+v)/2;
		g_Schrift.Schreiben(farbe,t1 , text);
		v-=v_start;		//v um v_start verschieben
	}

	float x=!v;

	if(x>0)
	{
	MatStapel.neu(MatTrans(v_start.x,v_start.y,v_start.z));

	if(v.z!=0 || v.y!=0)
		MatStapel.hinzu(MatRot(CVektor(0,-v.z,v.y),acos(v.x/x)/PI*180)); //Raum auf x-Achse drehen
	else
		if(v.x<0)
			MatStapel.hinzu(MatSkal(-1,1,1));

	MatStapel.hinzu(MatSkal(1,faktor,faktor));

	float a=0.7f;	//Pyramidenhöhe
	float b=0.3f;	//Pyramidenbreite/2
	float c=0.1f;	//Pfeildicke
		
	x-=a;
	//############# Pfeilspitze
	
	

	CVektor spitze,b_b,b_mb,mb_b,mb_mb;
	spitze=CVektor(a,0,0);
	b_b=CVektor(0,+b,+b);
	mb_mb=CVektor(0,-b,-b);
	b_mb=CVektor(0,+b,-b);
	mb_b=CVektor(0,-b,+b);

	


	MatStapel.neu(MatTrans(x,0,0));

	MatStapel.hinzu(MatSkal(faktor,1,1));

	for(short i=0;i<4;i++)
	{
		ZeichneDreieck(spitze,b_b,mb_b,farbe);
		MatStapel.hinzu(MatRot_X(90));
	}

	MatStapel.zurueck();


	
	//############# Pfeilspitze - Ende


	//############# Pfeilkörper


	ZeichneRechteck(b,b,x,0,farbe,-1);
	
	if(x>0)// (nur wenn Gesamtlänge größer als Pfeilspitzenlänge) x=x-a (s.o.)
	{

	ZeichneRechteck(c,c,0,0,farbe,-1);

	MatStapel.neu(MatTrans(x/2,0,0));
	for(short i=0;i<4;i++)
	{
		ZeichneRechteck(x/2,c,c,1,farbe);
		MatStapel.hinzu(MatRot_X(90));
	}

		
	
	MatStapel.zurueck();

	}

	MatStapel.zurueck();
	}

return;
}

void CVektor::Schreiben(bool rechts, int Zeile, bool iw)
{	
	if(iw)
		g_Schrift.Zeile(rechts,Zeile,0,"%.1f; %.1f; %.1f; %.1f",x,y,z,w);
	else
		g_Schrift.Zeile(rechts,Zeile,0,"%.1f; %.1f; %.1f",x,y,z);
}

