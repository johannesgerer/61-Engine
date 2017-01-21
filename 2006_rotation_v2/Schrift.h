//################################
//Schrift.h (letzte �nderung: 25.10.03 20:41)
//################################
/*
Diese Datei enth�lt

a) Klasse CSchrift. (Verantwortlich f�r Ausgabe von 2D-Schrift)
---------------------------------------
+ Sie muss Initialisiert werden nach der OpenGL Initialisierung: 
	Initialisieren(AnwendungsDaten *window); (Hier werden Schriftg��e und Art festgelegt)

+ Sie hat ein Array von Zeilen:
	-Diese konnen angesprochen werden mit:	Zeile(Zeilennummer, Farbe, Text)
	-Und ausgegeben werden mit:				ZeilenAusgeben()

+ Sie hat Funktionen zu direkten Ausgabe von Text:
	-Schreiben(..., CVektor		,...)	f�r Ausgabe im 3D-Raum
	-Schreiben(..., int x, int y,...)	f�r Ausgabe im 2D-Raum (x=0;y=0 enspricht links-oben)


b) stuct Zeile
---------------------------------------------
Dieser Typ repr�sentiert eine Zeile mit 255 Zeichen und einer Farbe
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
//Membervaiablen (gesch�tzt):
	Zeile Zeilen[20];	//Enth�lt 10 Zeilen zur Ausgabe in der Bildschirmecke links oben.
	Zeile ZeilenRechts[20];
	UINT ZeichenBasis;	//Diese Datei enth�lt die OpenGL Zeichen der entsprechenden Schrift
	int SchriftHoehe, FensterBreite;	//H�he der Schrift
	int FensterHoehe;	//H�he de OpenGL-Fensters

//Memberfunktionen (gesch�tzt):
	void Ausgeben2D(int farbe, int x, int y, char *text); 
	void Ausgeben3D(int farbe, CVektor pos, char* text);

	//Wird von Initialisieren aufgerufen und 
	//l�d die Schrift als OpenGL-List in den Speicher
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
	
	//Destrukor (L�scht die OpenGL-List mit den Zeichen)
	~CSchrift(void){glDeleteLists(ZeichenBasis, 96);}

	//Zum Aufruf durch "Funktionen.cpp" nach der OpenGL-Initialisierung
	//(Hier werden Schriftg��e und Art festgelegt)
	void Initialisieren(HDC* hDC,Viewport* viewport);

	//Wandelt "char *fmt" und die Parameter in einen g�ltigen Text um,
	//Deaktiviert die Berechnung von Licht f�r die Schrift
	//und ruft Ausgeben2D auf.
	//(x=0;y=0 enspricht links-oben)
	void Schreiben(int farbe, int x, int y, const char *fmt, ...);

	//Wandelt "char *fmt" und die Parameter in einen g�ltigen Text um,
	//Errechnet die Position des Text, der vor die �brigen Objekte ger�ckt werden muss,
	//und ruft Ausgeben3D auf.
	void Schreiben(int farbe, CVektor pos, const char *fmt, ...);
	
	//�ndert den Text/Farbe einer Zeile
	void Zeile(bool rechts, int i,int farbe, const char *fmt, ...);

	//Gibt alle Zeilen mit der Funktion "Ausgabe2D" am linken Bildschrim Rand aus.
	void ZeilenAusgeben();
};
