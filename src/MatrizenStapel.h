#pragma once
#include "Mathe.h"
class CMat;

class CMatrizenStapel
{
public:
	CMat	*Stapel, *mAnsichtp, *mProjektionp;

	//Anzahl:	Gesamtanzahl der Matrizen im Stapel (frei und belegt)
	//Top:		Index der "Obersten"
	//Gebraucht:Anzahl der genutzen Matrizen im Stapel
	int Anzahl, Top, Gebraucht;
		
	//Standardkonstruktor
	CMatrizenStapel();

	//Neue Matrix auf den Stapel legen
	void neu(CMat M=CMat());

	//Mulriplizieren der obersten Matrix mit M
	void hinzu(CMat M); 

	//Matrizen des Stapels löschen, und Zähler zurücksetzen
	//(wird vor jedem neuen Frame ausgerufen)
	void Reinigen();

	//oberste wird gelöscht
	void zurueck();
	
	//Gibt die aktelle Gesamtmatrix (Projektions-, Ansichts- und obersten Stapelmatrix)
	CMat Szene();

	//Gibt den Vektor muiltipliziert mit Projektions-, Ansichts- und obersten Stapelmatrix zurück
	CVektor Projektion(CVektor v);

	//Gibt den Vektor muiltipliziert mit Ansichts- und obersten Stapelmatrix zurück
	CVektor Ansicht(CVektor v);

	//Gibt den Vektor muiltipliziert mit obersten Stapelmatrix zurück
	CVektor Modell(CVektor v);

	float * operator [] (CVektor v);
	CVektor operator () (CVektor v);
	CVektor operator () (float x, float y, float z, float w);

	//Ruft glNormal mit Ansichtkoordinaten auf
	void Normale(CVektor v);
	void Normale(float x, float y, float z);
	//Ruft glVertex mitprojizierten Koordinaten auf
	void Punkt(CVektor v);
	void Punkt(float x, float y, float z);

	operator double* ();
	operator CMat* ();
	operator CMat();
	
};
