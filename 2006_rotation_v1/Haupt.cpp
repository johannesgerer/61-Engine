#include "NeHeGL.h"
#include "Mathe.h"
#include "Farben.h"
#include "Kamera.h"
#include "Szene.h"
#include "Objekt.h"
#include "Transformationen.h"
#include "Licht.h"
#include "Schrift.h"
#include "MatrizenStapel.h"
#include "Koerper.h"
#include "Planet.h"


//#######    Initialisierungsbereich  (Wird einmal ausgef�hrt)  ###############

//Speichert Informationen (siehe NeHeGL.h)
extern AnwendungsDaten	Anwendung;

//um anderen *.cpp Dateien zugriff auf die Viewport-Eigenschaften zu verschaffen
Viewport*				viewport;		

//Die Szeneklasse enth�lt alle Objekte und ruft deren Aktualisierungs- und Zeichenfunktionen auf
CSzene					Szene(0,0);	

//Gibt allen CObjekten und CKameras die Adresse der Szene
CSzene					*CObjekt::m_Szene	=	&Szene;	
CSzene					*CKamera::m_Szene	=	&Szene;

//Die Kameras
CKamera					g_Kamera1(	CVektor(0	,0	,1)	,		//Oben
									CVektor(10) ,	//Position
									CVektor(0,0	,0	,1)	,	//Ziel	
									1, 90, 1,1);		//Zu anfangs aktive Kamera

CKamera					g_Kamera2(	CVektor(0	,1	,0)	,			//Oben
									CVektor(0	,0	,30	,1) ,	//Position
									CVektor(0	,0	,0	,1),		//Ziel	
									0,	45,	1, 1,1	);	

CKamera					g_Kamera3(	CVektor(0	,0	,1)	,			//Oben
									CVektor(-14.0	,-8.8	,8.3	,1)*2 ,	//Position
									CVektor(0,0	,0	,1),		//Ziel	
									 0,	45,	1,1,0	);	


//Licht
CLicht					g_Licht	(	CVektor(1,1,3)	,	//Position
									0.6			,	//Direktlichtst�rke
									0.6			);	//Umgebungslichtst�rke

//MatrizenStapel, der die Projektions-, Ansichts- und Objektmatrizen speichert, und zur�ckgibt, oder Punkte und Normalen in OpenGL aufruft
//�bergeben wird die Adresse der Ansichtsmatrix, die die g_Kamera bei bedarf ermittelt
CMatrizenStapel		MatStapel;	

//Eine Klasse mit vordefinierten Farben
CFarben				Farben;

//Schrift-Objekt
CSchrift	g_Schrift; 

//ein paar Variablen, die von mehreren Funktionen genutzt werden
double ZeitSchritt;double LaufZeit;
double speed=1;
DWORD start;	
bool Stopp;

void Tastenbelegung();void Rendern (void);double FPS();void ToggleEgo();void ToggleProjOpenGL();

//#########  START: Szene  #############
CSteuerbar Erde1		(CVektor(0,0,2,1), 31  , 6,1);
CObjekt R3 (CVektor(0,0,2,1),0,"");

void SzeneInitialisieren()
{
//Postitionen der Objekte festlegen 
//(muss hier geschehen, da sich die Positionen �ndern wieder zur�ckgesetzt werden k�nnen m�ssen)
	Erde1.vPosition		= CVektor(0,0,2,1);

//Simulation am Anfang noch stoppen
	Stopp=true;
	speed=1;

//Zu Zeichnende Objekte f�r die Objekte der Szene festlegen
	Erde1.Schiff(5,10,2 );
	R3.R3(5);

//Start-Werte festlegen
	Erde1.vGeschwindikkeit=CVektor(0,0,0);
	//Erde1.vDrehimpuls=CVektor(1,20,1,0);

//Position der Kamera 1 auf den Satellit setzen
	g_Kamera1.PositionsObjektSetzen(&Erde1,CVektor(-0.5,0.5,3,1));

//Ziele der anderen Kameras festlegen
	g_Kamera1.vZiel=Szene.Schwerpunkt.vPosition;
	g_Kamera2.Ziel=&Szene.Schwerpunkt.vPosition;
	g_Kamera3.ZielNr=-1;
	g_Kamera3.naechstesZiel();

//Anfangs Energien ausrechnen
	Szene.InitEnergie();

//Zeit speichern
	start=GetTickCount();

//Position des Lichs setzen
	g_Licht.vPosition=Szene.Schwerpunkt.vPosition;
}

//#########  ENDE: Szene  #############


//#######    Programmablauf    ################################################
//(Wird von der Programmschleife in der Winmain aufgerufen)

//Gemischte Handlungen, die bei jedem Frame get�tigt werden
void Gemischt()
{
	
	CVektor K(1,5,0);
	CVektor r(0,5,0);

	Erde1.Traegheitstensor.Schreiben(0,9);

	Erde1.vWinkelG.Schreiben(0,6);
	g_Schrift.Zeile(0,7,0,"%.3f; %.3f; %.3f",Erde1.vDrehimpuls.x,Erde1.vDrehimpuls.y,Erde1.vDrehimpuls.z);
	//Erde1.vDrehimpuls.Schreiben(0,7);

	if(Erde1.E_kin_0==0)
		Erde1.E_kin_0=Erde1.vDrehimpuls*(Erde1.Orientierung*!Erde1.Traegheitstensor*!Erde1.Orientierung*Erde1.vDrehimpuls)/2;

	g_Schrift.Zeile(0,16,0,"E_Rot: %.5f",Erde1.vDrehimpuls*Erde1.vWinkelG/2/Erde1.E_kin_0);
	
	g_Schrift.Zeile(0,17,0,"Orient: %.15f, %.15f, %.15f, ",!Erde1.Orientierung(0),!Erde1.Orientierung(1),!Erde1.Orientierung(2));

	g_Schrift.Zeile(0,18,0,"Orient: %.15f, %.15f, %.15f, ",Erde1.Orientierung(0)*Erde1.Orientierung(1),Erde1.Orientierung(1)*Erde1.Orientierung(2),Erde1.Orientierung(0)*Erde1.Orientierung(2));

	R3.Orientierung=Erde1.Orientierung;

	//Erde1.paar(K,r,0);

	
		

	MatStapel.neu(MatTrans(Erde1.vPosition));

		VektorZeichnen(3,CVektor(),Erde1.vDrehimpuls,0,"L");
		VektorZeichnen(1,CVektor(),Erde1.vWinkelG*5,0,"W");
		
		MatStapel.hinzu(Erde1.Orientierung);

		VektorZeichnen(2,r,K,0,"K");	
		
		VektorZeichnen(19,CVektor(),r,1,"r");

		MatStapel.zurueck();


	
}

//Es kann �berpr�ft werden, ob eine Taste gedr�ckt wurde ("Ereignis") oder ob sie gehalten wird ("Unten")
void Tastenbelegung()
{
	
//Kamerabewegung
	double faktor=50;

		Erde1.gerade(Anwendung.Taste.Unten['W']);
		Erde1.bremse(Anwendung.Taste.Unten['S']);
		Erde1.links(Anwendung.Taste.Unten['A']);
		Erde1.rechts(Anwendung.Taste.Unten['D']);

		
//�ber die Tasten 1-9 hat man Zugriff auf alle Kameras
	for(short i=0;i<9;i++)
		if (Anwendung.Taste.Ereignis [49+i])	Szene.naechsteKamera(i);

//Zeitsteuerung
	if (Anwendung.Taste.Ereignis [VK_INSERT])		{speed+=0.5f*speed;}	//Schneller
	if (Anwendung.Taste.Ereignis [VK_DELETE])	{speed-=0.5f*speed;}	//Langsamer
	if (Anwendung.Taste.Ereignis [VK_MULTIPLY])	{speed=-speed;}			//R�ckw�rts
	if (Anwendung.Taste.Ereignis [VK_SPACE])	Stopp=!Stopp;	//Zeitanhalten

//Zoom
	if (Anwendung.Taste.Unten	[VK_PRIOR])	{	Szene.KameraOeffnungswinkel(-(float)ZeitSchritt*50);ProjektionAktualisieren();}
	if (Anwendung.Taste.Unten	[VK_NEXT])	{	Szene.KameraOeffnungswinkel((float)ZeitSchritt*50) ;ProjektionAktualisieren();}

//Beenden
	if (Anwendung.Taste.Ereignis [VK_ESCAPE])	TerminateApplication();

//Wireframe Modus
	if (Anwendung.Taste.Ereignis [VK_LBUTTON])		TogglePolygonMode();	

//Vollbild
	if (Anwendung.Taste.Ereignis [VK_F1])		ToggleFullscreen();
// Umschalten von Projektion durch OpenGL oder durch eigene Projektionsmatrix
	if (Anwendung.Taste.Ereignis [VK_F2])		ToggleProjOpenGL();
//Farbe anzeigen
	if (Anwendung.Taste.Ereignis ['C'])		ToggleColorMode();
//Normalen der selbst erstellten Fl�chen anzeigen
	if (Anwendung.Taste.Ereignis [VK_F4])		Anwendung.viewport.normalen_anzeigen=!Anwendung.viewport.normalen_anzeigen;
//Kraft und Geschwindigkeit anzeigen
	if (Anwendung.Taste.Ereignis [VK_F3])		Szene.VektorenAnzeigen=!Szene.VektorenAnzeigen;
//aktive Kamera:
	//Auf Ego-Sicht umschalten (Drehung um eigene Achse, nicht um Ziel)
	if (Anwendung.Taste.Ereignis [VK_F5])		ToggleEgo();
	//N�chstes Objekt der Szene als Ziel w�hlen
	if (Anwendung.Taste.Ereignis [VK_RBUTTON])		Szene.naechstesZiel();

//Szene nocheinmal von Vorne starten lassen
	if (Anwendung.Taste.Ereignis [VK_F11])		SzeneInitialisieren();


	if (Anwendung.Taste.Unten	[VK_UP])
	{
		Erde1.paar(CVektor(10,0,0),CVektor(0,1,2),0);
	}
	if (Anwendung.Taste.Unten	[VK_DOWN])	
	{
		Erde1.paar(CVektor(-10,0,0),CVektor(0,1,2),0);
	}
	if (Anwendung.Taste.Unten	[VK_LEFT])		
	{
		Erde1.paar(CVektor(0,10,0),CVektor(0,0,4),0);
	}
	if (Anwendung.Taste.Unten	[VK_RIGHT])
	{
		Erde1.paar(CVektor(0,-10,0),CVektor(0,0,4),0);
	}

	Anwendung.Taste.EreignisseLeeren();
}

//Informationen, die in jedem Frame eingeblendet werden
void TextInformationen()
{
	
	g_Schrift.Zeile(1,1,0,"FPS:   %.1f",FPS());
	g_Schrift.Zeile(1,2,0,"Zeit:   %.1f",LaufZeit);

	g_Schrift.Zeile(1,4,0,"Kamera:  %d",Szene.aktive_Kamera+1);
	g_Schrift.Zeile(1,5,0,"FOV:   %.1f",(Szene.aktiveKamera())->Oeffnungswinkel);
	g_Schrift.Zeile(1,6,0,"Geschwindigkeit:");
	(Erde1.vGeschwindikkeit).Schreiben(1,7);
	g_Schrift.Zeile(1,8,0,"Kraft:");
	(Erde1.Kraft).Schreiben(1,9);

	g_Schrift.Zeile(1,11,0,"Speed %.2f 1/s",speed);

	if(Stopp)
		g_Schrift.Zeile(1,12,0,"Stopp");
	else
		g_Schrift.Zeile(1,12,0,"");

	if(viewport->ProjOpenGL)
		g_Schrift.Zeile(1,14,0,"Projektion:  OpenGL");
	else	
		g_Schrift.Zeile(1,14,0,"Projektion:  eigene");

	g_Schrift.ZeilenAusgeben();
}

void Update ()
{
//1. Zeitberechnungen:
	static	DWORD letzteZeit;
			DWORD aktuelleZeit	=	GetTickCount();
	ZeitSchritt=0.001*double(aktuelleZeit-letzteZeit);
	letzteZeit=aktuelleZeit;

	static bool nichterstesframe; //wird mit false Initialisiert
	if(!nichterstesframe)
		ZeitSchritt=0;
	nichterstesframe=true;	

	

//2. Tasten �berpr�fen
	Tastenbelegung();

//3.  Physikalische Berechnungen
	if(Stopp)
		Szene.PhysikAktualisieren(0);
	else
	{
		LaufZeit+=ZeitSchritt*speed;
		Szene.PhysikAktualisieren(ZeitSchritt*speed);
	}

//4. Die Scene und Depth Buffer entleeren
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
		
//5. Matrizen aktualisieren:
	Szene.KameraAnsichtAktualisieren();
	MatStapel.Reinigen();
	MatStapel.mAnsichtp=&(Szene.aktiveKamera())->Ansicht;
	MatStapel.mProjektionp=&(Szene.aktiveKamera())->Projektion;

//6. Licht aktualisieren:
	g_Licht.Leuchte();	
	
//7. Objekte der Szene ausgeben:
	Szene.Zeichnen();
	
	Gemischt();	

//8. Informationen einblenden
	TextInformationen();
	Szene.EnergieAusgeben();
	
	glFlush ();					// Flush The MatStapel Rendering Pipeline
}

double FPS()
{
	static double Zeit;		//Speichert die vergange Zeit seit der letzten Berechnung
	static int	Frames;		//Speichert die Anzahl der Frames seit der Letzten Berechnung
	static double	fps;	//Speichert den letzen Wert seit der letzten Berechnung			

	Frames++;		
	Zeit+=ZeitSchritt;

	if(Zeit>0.4)	//Wenn schon 0.4 Sekunde(n) vergangen ist(sind)
	{
		fps=Frames/Zeit;	//neuen Wert berechnen
		Frames=0;	
		Zeit=0;
	}

	return fps;
}

void ToggleEgo()
{
	if((Szene.aktiveKamera())->PositionsObjekt==0)	
		(Szene.aktiveKamera())->m_Ego=!(Szene.aktiveKamera())->m_Ego;

	if((Szene.aktiveKamera())->m_Ego)
		Szene.naechstesZiel();
}

void ToggleProjOpenGL()
{
	Anwendung.viewport.ProjOpenGL=!Anwendung.viewport.ProjOpenGL;
	ProjektionAktualisieren();
}

void ProjektionAktualisieren()
{
	if(viewport==0)
		viewport=&Anwendung.viewport;
	else
		Szene.KameraProjektionAktualisieren();
}

void ToggleFullscreen ()								// Toggle Fullscreen/Windowed
{
	PostMessage (Anwendung.window.hWnd, 0x0400+1, 0, 0);				// Send A WM_TOGGLEFULLSCREEN Message
	SzeneInitialisieren();
}

bool Initialisierung ()
{
	viewport=&Anwendung.viewport;

	int MitteX = Anwendung.viewport.breite  >> 1;				// Setzt die Maus in die Mitte
	int MitteY = Anwendung.viewport.hoehe >> 1;	
	SetCursorPos(MitteX, MitteY);	
		
	glClearColor (0.7,0.7,0.7,0);							            // Black Background
	glClearDepth (1);											// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);										//Johannes Gerer
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			//Johannes Gerer
	glEnable (GL_LIGHTING);										//Johannes Gerer
	glEnable(GL_COLOR_MATERIAL);								//Johannes Gerer
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glEnable(GL_LINE_SMOOTH);

	Szene.KameraMitteEinstellen(MitteX,MitteY);
	g_Schrift.Initialisieren(&Anwendung.window.hDC,viewport);
	SzeneInitialisieren();									
	return 1;
}