#pragma once
#include "Mathe.h"
#include "Objekt.h"

class CKoerper : public CObjekt
{
public:

	CVektor Moment;		//Nur um Werte, nach der Aktualisieren noch zu haben
	CVektor Kraft;		//Nur um Werte, nach der Aktualisieren noch zu haben
	CVektor vGeschwindikkeit,Geschwindikkeit;
	CVektor vDrehimpuls;
	CVektor vKraft;
	CVektor vMoment;
	CVektor winkelge;

	CVektor F;
	CMat Traegheitstensor;

	double E_kin_0,Masse,E_pot_0,c;
	bool Physik, VektorenAnzeigen,Bremse;

	CKoerper():CObjekt(CVektor(),0){vGeschwindikkeit=CVektor(0,0);Physik=0;c=0;};

	CKoerper(CVektor p, int i_farbe, double i_Masse,double i_c=0,char* itext=0, bool i_Physik=1, bool i_VektorenAnzeigen=0, bool i_sichtbar=true, bool i_licht=true);

	void Zeichnen(bool Vektoren);

	void PhysikAktualisieren(double sekunden);
	void hinzu(CVektor F, CVektor r,bool ObjektKoordinaten=1);
	void paar(CVektor F, CVektor r,bool ObjektKoordinaten=1);
	double InitEnergie();

	double E_kin();double E_pot();
};

class CSteuerbar: public CKoerper
{
public:
	CSteuerbar(CVektor p,int i_farbe, float i_Masse,double i_c=0,char* itext=0, bool i_VektorenAnzeigen=0, bool i_sichtbar=true, bool i_licht=true):CKoerper(p, i_farbe, i_Masse, i_c,itext,1,i_VektorenAnzeigen,i_sichtbar){};

	void rechts(bool an=0);
	void links(bool an=0);
	void gerade(bool an=0);
	void bremse(bool an);
};