#include "MatrizenStapel.h"
#include "Mathe.h"
#include "Transformationen.h" 
#include "GLInclude.h"

CMatrizenStapel::CMatrizenStapel()
{
	Stapel=new CMat[10];
	Anzahl=10;
	Top=0;
	Gebraucht=1;
}

void CMatrizenStapel::hinzu(CMat M)
{

	Stapel[Top]=Stapel[Top]*M;
}

void CMatrizenStapel::Reinigen()
{
	delete [] Stapel;		//alten Stapel löschen
	Stapel=new CMat[Gebraucht];	//neuen Stapel (Größe=Anzahl gespeicherter Matrizen im letzen Frame)
	Anzahl=Gebraucht;			
	Top=0;
	Gebraucht=1;
	*Stapel=CMat();	
}

void CMatrizenStapel::zurueck()
{
	Top--;
}

void CMatrizenStapel::neu(CMat M)
{

	if(Anzahl==Top+1)	//Wenn der alte Stapel ganz voll ist
	{
		CMat *neuerStapel=new CMat[Anzahl+10];	//neuen Stapel mit 10 Elementen mehr
		for(int i=0;i<Anzahl;i++)					
			*neuerStapel++=*Stapel++;			//Matrizen aus dem alten Stapel kopieren
		
		delete [] (Stapel-Anzahl);				//alten Stapel löschen
		Stapel=(neuerStapel-Anzahl);	//den Zeiger des Stapels auf den Anfang des neuen schieben

		Anzahl+=10;
	}

	Gebraucht++;

	Stapel[++Top]=Stapel[Top-1]*M;
}

CMat CMatrizenStapel::Szene()
{
	return *mProjektionp**mAnsichtp*Stapel[Top];
}


CVektor CMatrizenStapel::Projektion(CVektor v)
{
	return *mProjektionp*Ansicht(v);
}

CVektor CMatrizenStapel::Ansicht(CVektor v)
{
	return *mAnsichtp*Modell(v);
}

CVektor CMatrizenStapel::Modell(CVektor v)
{
	return Stapel[Top]*v;
}

CVektor CMatrizenStapel::operator () (CVektor v)
{
		return (Stapel[Top]*v);
}

CVektor CMatrizenStapel::operator () (float x, float y, float z, float w)
{
	return (this->operator ()(CVektor(x,y,z,w)));
}

void CMatrizenStapel::Normale(float x, float y, float z)
{
	Normale(CVektor(x,y,z,0));
}

void CMatrizenStapel::Normale(CVektor n)
{
	n.w=0;
	n=Ansicht(n);
	glNormal3d(n.x,n.y,n.z);
}

void CMatrizenStapel::Punkt(float x, float y, float z)
{
	Punkt(CVektor(x,y,z));
}

void CMatrizenStapel::Punkt(CVektor p)
{
	p.w=1;
	p=Projektion(p);
	glVertex4d(p.x,p.y,p.z,p.w);
}

CMatrizenStapel::operator CMat()
{
	return Stapel[Top];
}

