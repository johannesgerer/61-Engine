#include "Schrift.h"
#include "Mathe.h"
#include "Farben.h"
#include "MatrizenStapel.h"
#include "NeHeGL.h"
#include "Transformationen.h"
#include "Szene.h"
#include "Kamera.h"

extern CFarben Farben;
extern CMatrizenStapel MatStapel;
extern Viewport* viewport;
extern CSzene Szene;

void CSchrift::Initialisieren(HDC* hDC, Viewport* viewport)
{
	FensterHoehe=viewport->hoehe;
	FensterBreite=viewport->breite;

	SchriftHoehe=20;		//Schriftgröße
	Erstellen(SchriftHoehe,
			  0,			//Breite (ändert nichs?!)
			  FW_BOLD,		//Stärke (FW_NORMAL=400, FW_BOLD=700, FW_HEAVY=900, FW_DONTCARE=0
			  false,		//Krusiv
			  false,		//Unterstrichen
			  false,		//Durchgestrichen
			  "MS Mincho",	//Name
			  hDC);
}

void CSchrift::ZeilenAusgeben()
{
	glDisable(GL_LIGHTING);
	
	for(int i=0; i<19; i++)
	{
			Ausgeben2D(Zeilen[i].farbe, 5,(i*SchriftHoehe+2), Zeilen[i].Text);
			Ausgeben2D(ZeilenRechts[i].farbe, FensterBreite- 11*(int)strlen(ZeilenRechts[i].Text),i*(SchriftHoehe+2), ZeilenRechts[i].Text);
	}
	glEnable(GL_LIGHTING);
}

void CSchrift::Zeile(bool rechts, int i,int farbe, const char *fmt, ...)
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
void CSchrift::Ausgeben2D(int farbe, int x, int y, char *text)
{
	//glDisable(GL_LIGHTING);
	glColor4fv(Farben(farbe));
	glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	glMatrixMode( GL_PROJECTION );						// Projectionsmatrix Modus
	glPushMatrix();										// Neue Matrix
	glLoadIdentity();									// Einheitsmatrix
	
	glMatrixMode( GL_MODELVIEW );						// Modelview Matrix Modus
	glPushMatrix();										// Neue Matrix
	glLoadIdentity();									// Einheitsmatrix
	y = FensterHoehe - SchriftHoehe - y;				// Umrechnen, so das Urspung links oben und y-Achse nach unten zeigt
	glViewport( x - 1, y - 1, 0, 0 );					// Viewport setzen
	glRasterPos3f(0,0,-1);								// Vor der Rest setzen
	glPopMatrix();										// Modelview Marix löschen

	glMatrixMode( GL_PROJECTION );						// Projectionsmatrix Modus
	glPopMatrix();										// Pop the projection matrix off the stack

	glPopAttrib();		
	
	glPushAttrib(GL_LIST_BIT);
	glListBase(ZeichenBasis);							// Zeichen laden
	
	glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);	//Ausgeben
	glPopAttrib();	

	//glEnable(GL_LIGHTING);
}

void CSchrift::Schreiben(int farbe, int x, int y, const char *fmt, ...)
{
	char text[256];
	//Start -- String umwandel
	va_list		ap;										// Pointer To List Of Arguments
	va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);	
	//Ende--
	Ausgeben2D(farbe, x, y, text);
}

void CSchrift::Schreiben(int farbe, CVektor pos, const char *fmt, ...)
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

void CSchrift::Ausgeben3D(int farbe, CVektor pos, char* text)
{		
	glDisable(GL_LIGHTING);
	double off=1;

	float breite_halbe=viewport->zBildebene*tanf((Szene.aktiveKamera())->Oeffnungswinkel/360*PI);
	float verhaltnis=(float)viewport->breite/(float)viewport->hoehe;

	
	pos=MatStapel.Ansicht(pos);		//Postition mal Ansichtsmatrix	
	if(pos.z>=0)
		return;

	pos=~pos*1.1;					//Näher an die Kamera hohlen

	pos=CMat(	1/breite_halbe,	0,							0,							0,
					0,				1/breite_halbe*verhaltnis,	0,							0,
					0,				0,							0,							0,
					0,				0,							-1/viewport->zBildebene,	0	)
		*pos;

	pos/=pos.w;
	pos.w=1;
	
		pos=CMat(	viewport->breite/2,		0,					0,	0,
					0,						viewport->hoehe/2,	0,	0,
					0,						0,					1,	0,
					0,						0,					0,	1)
			*CMat(	1,	0,	0,	1,
					0,	-1,	0,	1,
					0,	0,	0,	0,
					0,	0,	0,	1)
			*pos;

		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

	if(farbe==0)
		farbe=16;

		Ausgeben2D(1,pos.x+off,pos.y+off,text);
		Ausgeben2D(farbe,pos.x,pos.y,text);
		

		glPopMatrix();
        
		glMatrixMode (GL_MODELVIEW);
		
		glEnable(GL_LIGHTING);
	//glDisable(GL_LIGHTING);			//Licht deaktivieren

	//	glPushMatrix();						//Neue Matrix
	//	glTranslated(pos.x, pos.y, pos.z);	//Raum zur Position der Schrift verschieben
	//	
	//		

	//		glColor4fv(Farben(1));			//Farbe

	//		
	//		//glRasterPos2d(0,0);					//Schrift in 2D auf dem Ursprung ausgeben
	//		//glPushAttrib(GL_LIST_BIT);			//neues BitList Attribut
	//		//glListBase(ZeichenBasis);			//Laden der BitList
	//		//glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);	// Ausgeben der Schrift
	//		//glPopAttrib();						//Bitlist Attribut entfernen

	//		//glColor4fv(Farben(farbe));			//Farbe
	//		//glRasterPos3d(0.002,0.002,0);					//Schrift in 2D auf dem Ursprung ausgeben
	//		//glPushAttrib(GL_LIST_BIT);			//neues BitList Attribut
	//		//glListBase(ZeichenBasis);			//Laden der BitList
	//		//glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);	// Ausgeben der Schrift
	//		//glPopAttrib();						//Bitlist Attribut entfernen

	//	glPopMatrix();						//Matrix entfernen

	//	glEnable(GL_LIGHTING);			
	//	
	//	glPopMatrix();
	//	
	//	glMatrixMode (GL_MODELVIEW);
	//	
	//	glLoadIdentity ();//Licht aktivieren
	//}
	//else
	//{
	//	pos=MatStapel.Projektion(pos);
	//	pos/=pos.w;
	//	pos.w=1;
	//	pos=CMat(	viewport->breite/2,		0,					0,	0,
	//				0,						viewport->hoehe/2,	0,	0,
	//				0,						0,					1,	0,
	//				0,						0,					0,	1)
	//		*CMat(	1,	0,	0,	1,
	//				0,	-1,	0,	1,
	//				0,	0,	0,	0,
	//				0,	0,	0,	1)
	//		*pos;

	//	Ausgeben2D(farbe,pos.x,pos.y,text);
	//	Ausgeben2D(1,pos.x+off,pos.y+off,text);
	//			
	//}
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