#include "NeHeGL.h"
#include "Farben.h"
#include "Kamera.h"
#include "Welt.h"
#include "Bauen.h"
#include "Licht.h"
#include "Schrift.h"
#include "LGS.h"

GL_Window*	g_window;
CTasten*	Taste;
CKamera		g_Kamera;
CLicht		g_Licht;
extern CSchrift	g_Schrift(&g_Kamera.m_vPosition);

CBauen Bauen;
CWelt Welt;

float q,w,e,r,t,z,u,i,o,angle;	//Ein paar Variablen zu Testzwecken

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
	g_Kamera.MausBewegung(&g_window->init);

	//5. Zeichen
	Draw();

}

void Draw (void)
{
//Brechnungen:
	CVektor a=CVektor( 9+q, 9+w, 0+e);		//Vektor zu Punkt A
	CVektor b=CVektor(-6+r, t+3, 9+z);	//Vektor zu Punkt B
	CVektor c=CVektor( 0+u,-6+i,-6+o);		//Vektor zu Punkt C

	CVektor ac	=	c-a;				//Verbindungsvektor AC
	CVektor ab	=	b-a;				//Verbindungsvektor AB
	CVektor n	=	ab^ac;				//Normale zum Dreieck ABC	
	
	CVektor m=	LGS4loesen(	CVektor(n.x,ab.x,ac.x),
							CVektor(n.y,ab.y,ac.y),
							CVektor(n.z,ab.z,ac.z),
							CVektor(n*a,a*b-a*a+(ab*ab)/2,a*c-a*a+(ac*ac)/2));
		//M(x,y,z) ist Schnittpunkt der 3 Ebenen:
		//x*n.x		+	y*n.y	+	z*n.z	=	m.x*n*a				(Ebene des Dreiecks)
		//x*ab.x	+	y*ab.y	+	z*ab.z	=	a*b-a*a+(ab*ab)/2	(Mittelsenkrechte Ebene zu AB)
		//x*ac.x	+	y*ac.y	+	z*ac.z	=	a*c-a*a+(ac*ac)/2,	(Mittelsenkrechte Ebene zu AC)

//Brechnungen Ende
		
	g_Kamera.m_vZiel=m; //Den K-Mittelpunkt der Kamera als Ziel zuweißen
	g_Kamera.Schaue();	//Kamera aktualisieren
	Bauen.Los();		//Gitter zeichnen

//Rendern & Schriftausgabe:

	g_Schrift.Zeile(1,3,Weiss,"vBlick: %.1f",!(g_Kamera.m_vZiel-g_Kamera.m_vPosition));
	g_Schrift.Schreiben(Gelb,m,"M");

	Bauen.KugelZeichnen(0.2,&m,Gelb,10,10);		//Mittelpunkt als Kugeldarstellen
	Bauen.KreisZeichnen(!(m-a),&m,&n,Rot,7);	//Kreis Zeichnen

	

		glPolygonMode(GL_FRONT,GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);

	//Bauen.KugelZeichnen(!(m-a),&m,Gelb,50,50);		//Mittelpunkt als Kugeldarstellen

	glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonMode(GL_BACK,GL_FILL);

	Bauen.Dreieck( &a, &b, &c,Blau);					//Dreieck Rendern

	//Punkt-Kooridnaten ausgeben
	g_Schrift.Zeile(0,1,Weiss,"A: %.1f ; %.1f ; %.1f", a.x,a.y,a.z); 
	g_Schrift.Zeile(0,2,Weiss,"B: %.1f ; %.1f ; %.1f", b.x,b.y,b.z);
	g_Schrift.Zeile(0,3,Weiss,"C: %.1f ; %.1f ; %.1f", c.x,c.y,c.z);
	g_Schrift.Zeile(0,4,Weiss,"AB: %.1f ; AC: %.1f ; BC: %.1f",!(b-a),!(c-a),!(b-c));
	g_Schrift.Zeile(0,5,Weiss,"M: %.1f ; %.1f ; %.1f", m.x,m.y,m.z);
	g_Schrift.Zeile(0,6,Weiss,"Radius: %.1f",!(m-a));
	g_Schrift.ZeilenSchreiben();
//Rendern & Schriftausgabe Ende

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

	g_window=window;											//Johannes Gerer
	g_Schrift.Initialisieren(window);							//Johannes Gerer
	Taste=Tasten;												//Johannes Gerer

	g_Licht.Leuchte();											//Johannes Gerer

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
 
	//if (Taste->Unten	[VK_RIGHT])		g_Kamera.rechts(0.05f*sekunden);

	//if (Taste->Unten	[VK_LEFT])		g_Kamera.rechts(-0.05f*sekunden);
	
	if (Taste->Ereignis ['C'])			ToggleColorMode();

	if (Taste->Ereignis ['Q'])	q+=0.5f;
	if (Taste->Ereignis ['A'])	q-=0.5f;
	if (Taste->Ereignis ['W'])	w+=0.5f;
	if (Taste->Ereignis ['S'])	w-=0.5f;
	if (Taste->Ereignis ['E'])	e+=0.5f;
	if (Taste->Ereignis ['D'])	e-=0.5f;
	if (Taste->Ereignis ['R'])	r+=0.5f;
	if (Taste->Ereignis ['F'])	r-=0.5f;
	if (Taste->Ereignis ['T'])	t+=0.5f;
	if (Taste->Ereignis ['G'])	q-=0.5f;
	if (Taste->Ereignis ['Z'])	z+=0.5f;
	if (Taste->Ereignis ['H'])	q-=0.5f;
	if (Taste->Ereignis ['U'])	u+=0.5f;
	if (Taste->Ereignis ['J'])	u-=0.5f;
	if (Taste->Ereignis ['I'])	i+=0.5f;
	if (Taste->Ereignis ['J'])	i-=0.5f;
	if (Taste->Ereignis ['O'])	o+=0.5f;
	if (Taste->Ereignis ['L'])	o-=0.5f;

	Taste->EreignisseLeeren();

}
