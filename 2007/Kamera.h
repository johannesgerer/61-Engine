#pragma once
#include "NeheGL.h"
#include "Mathe.h"

class CKamera;
class CSzene;
class CObjekt;

class CKamera
{
public:

	static CSzene* m_Szene;

	bool m_Ego,fest,starr;

	CVektor vPosition, vZiel, vOben, vObjektPosition;
	CVektor* Ziel;
    
	CObjekt* PositionsObjekt;

	int ZielNr;

	CMat Ansicht,Projektion;

	float Oeffnungswinkel;

	void PositionsObjektSetzen(CObjekt* Objekt, CVektor ObjektPosition);

	CKamera(CVektor oben, CVektor Position, CVektor iZiel, bool EGO, 
		float iOeffnungswinkel=45, bool aktiv=0, bool ifest=0, bool istarr=0);
	
	void MausBewegung();
	void Aktualisieren(bool projektion=0);

	void naechstesZiel();

	void vor(float wert);
	void rechts(float wert);
	void hoch(float wert);
};
