#include "Szene.h"
#include "Objekt.h"
#include "Farben.h"
#include "NeHeGL.h"
#include "Mathe.h"
#include "Kamera.h"
#include "MatrizenStapel.h"
#include "Schrift.h"
#include "Transformationen.h"

extern CFarben			Farben;
extern CKamera			g_Kamera;
extern CMatrizenStapel	MatStapel;
extern Viewport*		viewport;
extern CSchrift			g_Schrift;

void CSzene::Schwerpunktsberechnungen()
{
	Schwerpunkt.vGeschwindikkeit=CVektor(0,0);
	Schwerpunkt.vPosition=CVektor();
	Schwerpunkt.Masse=0;

	for(int i=0;i<Anzahl_Koerper;i++)
		if(m_Koerper[i]->Physik)
		{
			Schwerpunkt.Masse+=m_Koerper[i]->Masse;
			Schwerpunkt.vPosition+=m_Koerper[i]->vPosition*m_Koerper[i]->Masse;
			Schwerpunkt.vGeschwindikkeit+=m_Koerper[i]->vGeschwindikkeit*m_Koerper[i]->Masse;
		}

	Schwerpunkt.vPosition/=Schwerpunkt.Masse;
	Schwerpunkt.vPosition.w=1;

	Schwerpunkt.vGeschwindikkeit/=Schwerpunkt.Masse;

	if(Schwerpunkt_in_Ruhe)
	{
		for(int i=0;i<Anzahl_Koerper;i++)
			if(m_Koerper[i]->Physik)
			m_Koerper[i]->vGeschwindikkeit-=Schwerpunkt.vGeschwindikkeit;

		Schwerpunkt.vGeschwindikkeit=CVektor(0,0);
	}
}

void CSzene::PhysikAktualisieren(double sekunden)
{
	Schwerpunktsberechnungen();

	int Balken=100;
	double sl=sekunden/Balken;
		for(int j=0;j<Balken;j++)
			for(int i=0;i<Anzahl_Koerper;i++)
				m_Koerper[i]->PhysikAktualisieren(sl);

	for(int i=0;i<Anzahl_Koerper;i++)
	{
		m_Koerper[i]->vKraft=CVektor();
		m_Koerper[i]->vMoment=CVektor();
	}

	Schwerpunktsberechnungen();
}

void CSzene::EnergieAusgeben()
{
	double E=0;
	for(int i=0;i<Anzahl_Koerper;i++)
		if(m_Koerper[i]->Physik)
			E+=m_Koerper[i]->E_kin()+m_Koerper[i]->E_pot();
	
	//	double E_pot=Mond.Masse*Erde.Masse*G*(1/r_InitEnergie-1/!(Erde.vPosition-Mond.vPosition));

g_Schrift.Zeile(0,1,0,"Fehler/%%:  %e",100*E/Schwerpunkt.E_kin_0-100);

}
void CSzene::InitEnergie()
{
	PhysikAktualisieren(0);

	Schwerpunkt.farbe=farbe;

	Schwerpunkt.E_kin_0=0;

	for(int i=0;i<Anzahl_Koerper;i++)
		if(m_Koerper[i]->Physik)
			Schwerpunkt.E_kin_0+=m_Koerper[i]->InitEnergie();
	
}

void CSzene::Zeichnen()
{
	//########  Gitter  ##########

	if(1)
	{
	glColor4fv(Farben(9));
	MatStapel.Normale( 0, 0, 1);
	// Draw a 1x1 grid along the XY-Ebene
	MatStapel.neu(MatSkal(1));
	float k=-1;
	float a=50;
	glBegin(GL_LINES);
	for(float i=-a;i<=a;i++)
	{
		MatStapel.Punkt(-a, i, k);
		MatStapel.Punkt(a, i, k);	
		MatStapel.Punkt(i, -a, k);
		MatStapel.Punkt(i, a, k);
		
	}glEnd();
	MatStapel.zurueck();

	
	}
//	ENDE ########  Gitter  ##########

	for(int i=0;i<m_Anzahl;i++)
		m_Objekt[i]->Zeichnen();

	for(int i=0;i<Anzahl_Koerper;i++)
		m_Koerper[i]->Zeichnen(VektorenAnzeigen);
}

CSzene::CSzene(int i_farbe,bool i_Schwerpunkt_in_Ruhe)
{
	Schwerpunkt_in_Ruhe=i_Schwerpunkt_in_Ruhe;
	farbe=i_farbe;
	Schwerpunkt.text="Schwerpunkt";
}

CKamera* CSzene::aktiveKamera()
{
	return m_Kamera[aktive_Kamera];
}

void CSzene::naechsteKamera(short i)
{
	if(i!=-1){
		if(i<Anzahl_Kamera)
			aktive_Kamera=i;
		else
			return;
	}else{

		if(aktive_Kamera==Anzahl_Kamera-1)
			aktive_Kamera=0;
		else
			aktive_Kamera++;
	}

	if(viewport->ProjOpenGL)
		m_Kamera[aktive_Kamera]->Aktualisieren(1);
}

void CSzene::KameraProjektionAktualisieren()
{	
	if(viewport->ProjOpenGL)
		m_Kamera[aktive_Kamera]->Aktualisieren(1);
	else
		for(int i=0;i<Anzahl_Kamera;i++)
			m_Kamera[i]->Aktualisieren(1);
}

void CSzene::KameraAnsichtAktualisieren()
{
	m_Kamera[aktive_Kamera]->Aktualisieren();
}

void CSzene::KameraMitteEinstellen(int X, int Y)
{
	MitteX=X;MitteY=Y;
}

void CSzene::KameraOeffnungswinkel(float iOeffnungswinkel)
{
	m_Kamera[aktive_Kamera]->Oeffnungswinkel+=iOeffnungswinkel;
}

void CSzene::naechstesZiel()
{
	m_Kamera[aktive_Kamera]->naechstesZiel();	
}
