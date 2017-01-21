#pragma once
#include "Koerper.h"

class CObjekt;
class CKamera;

class CSzene
{
public:
//Konstruktoren:
	CSzene(int i_farbe);					//Standardkonstruktor

//Membervaiablen:
	CObjekt *m_Objekt[10];		//Array mit Objekten
	CKoerper *m_Koerper[10];	//Array mit Koerpern
	float m_Schwerkraft;		//Schwerkraft in N
	int farbe;
	bool VektorenAnzeigen;

	CKoerper Schwerpunkt;

	CKamera	*m_Kamera[10];

	int m_Anzahl, Anzahl_Kamera, Anzahl_Koerper, aktive_Kamera, MitteX, MitteY;				//interner Zähler für Objekte

//Funkionen
	void Zeichnen();						//Ausgabe des Gitters und aller Objekte durch OpenGL

	void PhysikAktualisieren(double sekunden);		//Aktualisiert alle Objekte (Position, Geschwindigkeit,...)
											//anhand physikalischer Gesetze, sofern das Objekt ein Körper ist

	void Hinzu(CKoerper* i_Koerper)
		{m_Koerper[Anzahl_Koerper++]=i_Koerper;};	

	void Hinzu(CObjekt *i_Objekt)		//Speichert die Adresse eines Objekts in das Array und erhöht den Zähler
		{m_Objekt[m_Anzahl++]=i_Objekt;};

	void Hinzu(CKamera *i_Kamera, bool aktiv=0)		//Speichert die Adresse eines Objekts in das Array und erhöht den Zähler
	{if(aktiv){aktive_Kamera=Anzahl_Kamera;} m_Kamera[Anzahl_Kamera++]=i_Kamera; };

	void InitEnergie();
	void EnergieAusgeben();

//Kamera Funktionen:
	void naechsteKamera(short i=-1);
	void KameraAnsichtAktualisieren();
	void KameraProjektionAktualisieren();
	CKamera* aktiveKamera();
	void KameraMitteEinstellen(int X, int Y);
	void KameraOeffnungswinkel(float iOeffnungswinkel);
	void naechstesZiel();
};

enum Typ	//Alle möglichen Erscheinungstypen, für ein Objekt
{
		tQuader=1,
		tR3,
		tPyramide,
		tKreis,
		tKugel,
		tMine,
		tSchiff
};

