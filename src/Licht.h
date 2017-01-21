//################################
//Licht.h (letzte Änderung: 23.10.03 17:02)
//################################
/*
Klasse CLicht
---------------------------------------
Für die Nutzung von Licht in der Scene:
- Die Lichtwerte werden in dem Konstruktor festgelegt
- Die Funktion "Leuchte" muss für jedes Frame aufgerufen werden, (nach Kamera.schaue)
	wenn die Möglichkeit besteht, dass sich Werte ändern.
	Ansonsten reicht ein Aufrauf in beim Initialisieren.
*/

#pragma once

#include "Vektor.h"

class CLicht
{
public:

//Membervariablen:
	CVektor vPosition;
	float Direkt, Umgebung;
	bool an;

//Standardkonstruktor
	CLicht(CVektor Position, float iDirekt, float iUmgebung);

//Memberfunktion
	//Aktiviert und/oder aktualisiert das Licht
	void Leuchte();
};