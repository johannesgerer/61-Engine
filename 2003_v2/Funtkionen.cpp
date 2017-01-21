#include "NeHeGL.h"
#include "Farben.h"
#include "Kamera.h"
#include "Welt.h"
#include "Bauen.h"
#include "Licht.h"
#include "Schrift.h"

GL_Window*	g_window;
CTasten*	Taste;
CKamera		g_Kamera;
CLicht		g_Licht;
extern CSchrift	g_Schrift(&g_Kamera.m_vPosition);

CBauen Bauen;
CWelt Welt;

float l,o,k,h,angle;	//Ein paar Variablen zu Testzwecken

void Tastenbelegung(float sekunden);
void Draw (void);

float FPS()
{
	static DWORD	Zeit;			//Speichert den Zeitpuntk der letzten Berechnung
	static int		Frames;			//Speichert die Anzahl der Frames seit der Letzten Berechnung
	static float	fps;			//Speichert den letzen Wert seit der letzten Berechnung
	DWORD			tickCount = GetTickCount(); //Gibt die aktuelle Zeit an

	Frames++;

	if(Zeit==0 || tickCount-Zeit>1000)	//Wenn schon 0.5 Sekunden vergangen sind
	{
		fps=(float)Frames/(tickCount-Zeit)*1000;	//neuen Wert berechnen
		Zeit=tickCount;								//Zeit aktualisieren
		Frames=0;									
	}

	return fps;
}

void InformationenEinblenden()
{
g_Schrift.Zeile(1,1,Weiss,"FPS: %.1f",FPS());
g_Schrift.Zeile(1,2,Weiss,"FOV: %.1f",g_window->init.FOV);
}

void Update (DWORD MilliSekunden)
{
	float sekunden=(float)MilliSekunden;

	//1. Die Scene und Depth Buffer entleeren
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//2. Informationen einblenden
	InformationenEinblenden();

	//3. Tasten überprüfen
	Tastenbelegung(sekunden);

	//4. Gemischte Handlungen
	Bauen.angle(-sekunden/ 10.0f);
	o+=sekunden/10;

	//5. Zeichen
	g_Kamera.MausBewegung(&g_window->init);
	Draw();

}

void Draw (void)
{	
	CVektor n=CVektor(0,0,0);

	g_Kamera.Schaue();

	CVektor v1=g_Kamera.m_vZiel-g_Kamera.m_vPosition;
	CVektor oben=~(v1^g_Kamera.m_vOben^v1);
	//VektorZeichnen(Orange,	CVektor(0,0,0),oben * 3	,0,"Oben");

	Bauen.Los();

	CVektor F=CVektor(5,0,0);
	CVektor r=CVektor(l,h,-5+k);
	CVektor N=r^F;

float	weg	=	!r;
CVektor f2=~(N^r)*(!N)/weg;
CVektor f	=	F;

if(weg>0)
	f=r*((r*F)/(weg*weg));

g_Schrift.Zeile(0,2,Weiss, "f1(%.1f,%.1f,%.1f)",f.x,f.y,f.z);
g_Schrift.Zeile(0,3,Weiss, "f2(%.1f,%.1f,%.1f)",f2.x,f2.y,f2.z);

VektorZeichnen(Orange,	n,	r,	false,	"r.z=%.1f, |r|=%.1f",r.z, weg);
VektorZeichnen(Gruen,	r,		F,	false,	"|Kraft|=%.1f"		,!F);
VektorZeichnen(Blau,	r,	N/3,	false,	"|N|=%.1f"			,!N);
VektorZeichnen(Lila,	r,	f2,	false,	"|f2|=%.1f"			,!f2);

VektorZeichnen(Rot,		n,	f,	false,	"|f1|=%.1f"			,!f);
	
	glTranslatef(g_Kamera.m_vPosition.x, g_Kamera.m_vPosition.y, g_Kamera.m_vPosition.z);
	Bauen.Kugeln(o, h, 3+l, CVektor(1,0,0)^oben, oben.x);
	glTranslatef(-g_Kamera.m_vPosition.x, -g_Kamera.m_vPosition.y, -g_Kamera.m_vPosition.z);
	
	g_Schrift.Schreiben(Gelb,g_Licht.m_vPosition,"Licht1");
	Bauen.Flaechen(g_Licht.m_vPosition);
	g_Schrift.ZeilenSchreiben();

	glFlush ();													// Flush The GL Rendering Pipeline
}

BOOL Initialisierung(GL_Window* window,CTasten* Tasten)
{
	int MitteX = window->init.width  >> 1;				// Setzt die Maus in die Mitte
	int MitteY = window->init.height >> 1;	
	SetCursorPos(MitteX, MitteY);
	glClearColor (0.0f, 0.0f, 0.0f, 1);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);										//Johannes Gerer
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			//Johannes Gerer
	glEnable (GL_LIGHTING);										//Johannes Gerer
	glEnable(GL_COLOR_MATERIAL);								//Johannes Gerer
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate

	g_window=window;
	g_Schrift.Initialisieren(window);
	Taste=Tasten;

	g_Licht.Leuchte();

	return 1;
}

void Deinitialisierung()
{}
void Tastenbelegung(float  sekunden)
{
	if (Taste->Unten [VK_PRIOR]){		g_window->init.FOV-=1;ReshapeGL(&g_window->init);}
	if (Taste->Unten [VK_NEXT]){		g_window->init.FOV+=1;ReshapeGL(&g_window->init);}

	if (Taste->Ereignis [VK_ESCAPE])	TerminateApplication(g_window);

	if (Taste->Ereignis [VK_F1])		ToggleFullscreen(g_window);

	if (Taste->Ereignis ['M'])			TogglePolygonMode();	

	if (Taste->Unten	[VK_UP])		g_Kamera.vor(0.05f*sekunden);

	if (Taste->Unten	[VK_DOWN])		g_Kamera.vor(-0.05f*sekunden);

	if (Taste->Unten	[VK_RIGHT])		g_Kamera.rechts(0.05f*sekunden);

	if (Taste->Unten	[VK_LEFT])		g_Kamera.rechts(-0.05f*sekunden);
	
	if (Taste->Ereignis ['C'])			ToggleColorMode();

	if (Taste->Unten ['A'])	l+=0.01f*sekunden;
	if (Taste->Unten ['D'])	l-=0.01f*sekunden;
	if (Taste->Unten ['W']) h+=0.01f*sekunden;
	if (Taste->Unten ['S'])	h-=0.01f*sekunden;
	if (Taste->Unten ['Q']) k+=0.005f*sekunden;
	if (Taste->Unten ['E']) k-=0.005f*sekunden;

	Taste->EreignisseLeeren();

}
