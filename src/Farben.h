//################################
//Farben.h (letzte Änderung (nicht Daten): 25.10.03 23:41)
//################################
/*
Diese Datei deklariert float-Datenfelder,
die RGB-Farbwerte gespeichert haben. 255 enspricht 100%
----------------------------------------------------
Diese Datenfelder werden für die folgende OpenGlFunktion benötigt:
glColor4fv(GLfloat *v). *v soll ein Pointer auf ein float-Datenfeld sein.
Beispiel: glColor4fv(Rot);
Mit Hilfe dieser Funktion hat man einfachen und 
übersichtlichen Zugriff auf elemtare Farben.
*/
#include "Vektor.h"

class CFarben
{
public:
	CFarben();
	float **farbe;
	int Top;
	float* operator ()(int i);
	CVektor operator [](int i);
	void hinzu(int f=0);
};


