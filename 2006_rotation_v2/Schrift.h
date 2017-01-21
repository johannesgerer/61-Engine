//################################
//Schrift.h (letzte Änderung: 25.10.03 20:41)
//################################
/*
Diese Datei enthält

a) Klasse CSchrift. (Verantwortlich für Ausgabe von 2D-Schrift)
---------------------------------------
+ Sie muss Initialisiert werden nach der OpenGL Initialisierung: 
	Initialisieren(AnwendungsDaten *window); (Hier werden Schriftgöße und Art festgelegt)

+ Sie hat ein Array von Zeilen:
	-Diese konnen angesprochen werden mit:	Zeile(Zeilennummer, Farbe, Text)
	-Und ausgegeben werden mit:				ZeilenAusgeben()

+ Sie hat Funktionen zu direkten Ausgabe von Text:
	-Schreiben(..., CVektor		,...)	für Ausgabe im 3D-Raum
	-Schreiben(..., int x, int y,...)	für Ausgabe im 2D-Raum (x=0;y=0 enspricht links-oben)


b) stuct Zeile
---------------------------------------------
Dieser Typ repräsentiert eine Zeile mit 255 Zeichen und einer Farbe
*/

#pragma once
#include <stdio.h>
#include <stdarg.h>	
#include "NeheGL.h"

struct Zeile
{
	char Text[256];
	int farbe;
};

class CVektor;
class CMat;

class CSchrift
{
//Membervaiablen (geschützt):
	Zeile Zeilen[20];	//Enthält 10 Zeilen zur Ausgabe in der Bildschirmecke links oben.
	Zeile ZeilenRechts[20];
	UINT ZeichenBasis;	//Diese Datei enthält die OpenGL Zeichen der entsprechenden Schrift
	int SchriftHoehe, FensterBreite;	//Höhe der Schrift
	int FensterHoehe;	//Höhe de OpenGL-Fensters

//Memberfunktionen (geschützt):
	void Ausgeben2D(int farbe, int x, int y, char *text); 
	void Ausgeben3D(int farbe, CVektor pos, char* text);

	//Wird von Initialisieren aufgerufen und 
	//läd die Schrift als OpenGL-List in den Speicher
	void Erstellen (int		Hoehe,
					int		Breite,
					int		Staerke,
					bool	kursiv, 
					bool	unterstrichen, 
					bool	durschgestrichen,
					LPSTR	Name,
					HDC		*hDC);
public:	

	CSchrift(){};
	
	//Destrukor (Löscht die OpenGL-List mit den Zeichen)
	~CSchrift(void){glDeleteLists(ZeichenBasis, 96);}

	//Zum Aufruf durch "Funktionen.cpp" nach der OpenGL-Initialisierung
	//(Hier werden Schriftgöße und Art festgelegt)
	void Initialisieren(HDC* hDC,Viewport* viewport);

	//Wandelt "char *fmt" und die Parameter in einen gültigen Text um,
	//Deaktiviert die Berechnung von Licht für die Schrift
	//und ruft Ausgeben2D auf.
	//(x=0;y=0 enspricht links-oben)
	void Schreiben(int farbe, int x, int y, const char *fmt, ...);

	//Wandelt "char *fmt" und die Parameter in einen gültigen Text um,
	//Errechnet die Position des Text, der vor die übrigen Objekte gerückt werden muss,
	//und ruft Ausgeben3D auf.
	void Schreiben(int farbe, CVektor pos, const char *fmt, ...);
	
	//Ändert den Text/Farbe einer Zeile
	void Zeile(bool rechts, int i,int farbe, const char *fmt, ...);

	//Gibt alle Zeilen mit der Funktion "Ausgabe2D" am linken Bildschrim Rand aus.
	void ZeilenAusgeben();
};
