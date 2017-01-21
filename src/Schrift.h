#pragma once
#include <stdio.h>
#include <stdarg.h>	
#include "Vektor.h"
#include "Mathe.h"

//################################
//Schrift.h (letzte Änderung: 25.10.03 20:41)
//################################
/*
Diese Datei enthält

a) Klasse CSchrift. (Verantwortlich für Ausgabe von 2D-Schrift)
---------------------------------------
+ Sie muss Initialisiert werden nach der OpenGL Initialisierung: 
	Initialisieren(GL_Window *window); (Hier werden Schriftgöße und Art festgelegt)

+ Sie hat ein Array von Zeilen:
	-Diese konnen angesprochen werden mit:	Zeile(Zeilennummer, Farbe, Text)
	-Und ausgegeben werden mit:				ZeilenSchreiben()

+ Sie hat Funktionen zu direkten Ausgabe von Text:
	-Schreiben(..., CVektor		,...)	für Ausgabe im 3D-Raum
	-Schreiben(..., int x, int y,...)	für Ausgabe im 2D-Raum (x=0;y=0 enspricht links-oben)


b) stuct Zeile
---------------------------------------------
Dieser Typ repräsentiert eine Zeile mit 255 Zeichen und einer Farbe
*/

struct Zeile
{
	char Text[256];
	float* farbe;
};


class CSchrift
{
//Membervaiablen (geschützt):
	Zeile Zeilen[10];	//Enthält 10 Zeilen zur Ausgabe in der Bildschirmecke links oben.
	Zeile ZeilenRechts[10];
	UINT ZeichenBasis;	//Diese Datei enthält die OpenGL Zeichen der entsprechenden Schrift
	int SchriftHoehe, FensterBreite;	//Höhe der Schrift
	int FensterHoehe;	//Höhe de OpenGL-Fensters
	CVektor *kam_pos;

//Memberfunktionen (geschützt):
	void Ausgeben2D(float *farbe, int x, int y, char *text); 
	void Ausgeben3D(float *farbe, CVektor pos, char* text);

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

	CSchrift(CVektor *ikam_pos){kam_pos=ikam_pos;};
	
	//Destrukor (Löscht die OpenGL-List mit den Zeichen)
	~CSchrift(void){glDeleteLists(ZeichenBasis, 96);}

	//Zum Aufruf durch "Funktionen.cpp" nach der OpenGL-Initialisierung
	//(Hier werden Schriftgöße und Art festgelegt)
	void Initialisieren(GL_Window *window);

	//Wandelt "char *fmt" und die Parameter in einen gültigen Text um,
	//Deaktiviert die Berechnung von Licht für die Schrift
	//und ruft Ausgeben2D auf.
	//(x=0;y=0 enspricht links-oben)
	void Schreiben(float *farbe, int x, int y, const char *fmt, ...);

	//Wandelt "char *fmt" und die Parameter in einen gültigen Text um,
	//Errechnet die Position des Text, der vor die übrigen Objekte gerückt werden muss,
	//und ruft Ausgeben3D auf.
	void Schreiben(float *farbe, CVektor pos, const char *fmt, ...);
	
	//Ändert den Text/Farbe einer Zeile
	void Zeile(bool rechts, int i,float *farbe, const char *fmt, ...);

	//Gibt alle Zeilen mit der Funktion "Ausgabe2D" am linken Bildschrim Rand aus.
	void ZeilenSchreiben();
};

void CSchrift::Initialisieren(GL_Window *window)
{
	FensterHoehe=window->init.height;
	FensterBreite=window->init.width;

	SchriftHoehe=20;		//Schriftgröße
	Erstellen(SchriftHoehe,
			  0,			//Breite (ändert nichs?!)
			  FW_BOLD,		//Stärke (FW_NORMAL=400, FW_BOLD=700, FW_HEAVY=900, FW_DONTCARE=0
			  false,		//Krusiv
			  false,		//Unterstrichen
			  false,		//Durchgestrichen
			  "MS Mincho",	//Name
			  &window->hDC);
}

void CSchrift::ZeilenSchreiben()
{
	glDisable(GL_LIGHTING);
	
	for(int i=0; i<10; i++)
	{
		if(Zeilen[i].farbe!=NULL)
			Ausgeben2D(Zeilen[i].farbe, 5,(i*SchriftHoehe+2), Zeilen[i].Text);
		if(ZeilenRechts[i].farbe!=NULL)
			Ausgeben2D(ZeilenRechts[i].farbe, FensterBreite- 11*(int)strlen(ZeilenRechts[i].Text),i*(SchriftHoehe+2), ZeilenRechts[i].Text);
	}
	glEnable(GL_LIGHTING);
}

void CSchrift::Zeile(bool rechts, int i,float *farbe, const char *fmt, ...)
{
	va_list		ap;	

	if(rechts)
	{
	//Start -- String umwandel							// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(ZeilenRechts[i-1].Text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--
	
	ZeilenRechts[i-1].farbe=farbe;
	}else{
	//Start -- String umwandel							// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(Zeilen[i-1].Text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--
	
	Zeilen[i-1].farbe=farbe;
	}
}
void CSchrift::Ausgeben2D(float *farbe, int x, int y, char *text)
{
	glColor4fv(farbe);
	glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	glMatrixMode( GL_PROJECTION );						// Set our matrix to our projection matrix
	glPushMatrix();										// Push on a new matrix to work with
	glLoadIdentity();									// reset the matrix
	glMatrixMode( GL_MODELVIEW );						// Set our matrix to our model view matrix
	glPushMatrix();										// Push on a new matrix to work with
	glLoadIdentity();									// Reset that matrix
	y = FensterHoehe - SchriftHoehe - y;				// Calculate the weird screen position
	glViewport( x - 1, y - 1, 0, 0 );					// Create a new viewport to draw into
	glRasterPos4f( 0, 0, 0, 1 );						// Set the drawing position
	glPopMatrix();										// Pop the current modelview matrix off the stack
	glMatrixMode( GL_PROJECTION );						// Go back into projection mode
	glPopMatrix();										// Pop the projection matrix off the stack
	glPopAttrib();		
	glPushAttrib(GL_LIST_BIT);
	glListBase(ZeichenBasis);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();	
}

void CSchrift::Schreiben(float *farbe, int x, int y, const char *fmt, ...)
{
	char text[256];
	//Start -- String umwandel
	va_list		ap;										// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--
	glDisable(GL_LIGHTING);
	Ausgeben2D(farbe, x, y, text);
	glEnable(GL_LIGHTING);
}

void CSchrift::Schreiben(float *farbe, CVektor pos, const char *fmt, ...)
{
	char		text[256];								// Holds Our String
	//Start -- String umwandel
	va_list		ap;										// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--	
	Ausgeben3D(farbe, pos, text);
}

void CSchrift::Ausgeben3D(float *farbe, CVektor pos, char* text)
{
	glDisable(GL_LIGHTING);
	
	CVektor nahe_pos=*kam_pos+(~(pos-*kam_pos))*1.1f;

	glColor4fv(farbe);
	glPushMatrix();
	glTranslatef(nahe_pos.x, nahe_pos.y, nahe_pos.z);
	glRasterPos2d(0,0);
	glPushAttrib(GL_LIST_BIT);				// Pushes The Display List Bits
	glListBase(ZeichenBasis);							// Sets The Base Character
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();	
	glPopMatrix();

	nahe_pos=*kam_pos+(~(pos-*kam_pos))*1.2f;
	glColor4fv(Schwarz);
	glPushMatrix();
	glTranslatef(nahe_pos.x, nahe_pos.y, nahe_pos.z);
	glRasterPos2f(0.001f,-0.001f);
	glPushAttrib(GL_LIST_BIT);				// Pushes The Display List Bits
	glListBase(ZeichenBasis);							// Sets The Base Character
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();	
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
}

void CSchrift::Erstellen(int Hoehe, int Breite, int Staerke, bool kursiv, bool unterstrichen,
						 bool durschgestrichen, LPSTR Name, HDC *hDC)
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping
	ZeichenBasis = glGenLists(240);								// Storage For 96 Characters
	font = CreateFont(	Hoehe,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						Staerke,						// Font Weight
						kursiv,							// Italic
						unterstrichen,					// Underline
						durschgestrichen,				// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						Name);							// Font Name
	oldfont = (HFONT)SelectObject(*hDC, font);          // Selects The Font We Want
	wglUseFontBitmaps(*hDC, 0, 256, ZeichenBasis);		// Builds 256 Characters
	SelectObject(*hDC, oldfont);						// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}