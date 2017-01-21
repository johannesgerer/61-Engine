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


//#######    Initialisierungsbereich  (Wird einmal ausgeführt)  ###############

//Speichert Informationen (siehe NeHeGL.h)
extern AnwendungsDaten	Anwendung;

//um anderen *.cpp Dateien zugriff auf die Viewport-Eigenschaften zu verschaffen
Viewport*				viewport;		

//Die Szeneklasse enthält alle Objekte und ruft deren Aktualisierungs- und Zeichenfunktionen auf
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
									CVektor(0	,0	,300	,1) ,	//Position
									CVektor(0	,0	,0	,1),		//Ziel	
									0,	45,	1, 1,1	);	

CKamera					g_Kamera3(	CVektor(0	,0	,1)	,			//Oben
									CVektor(-140	,-88	,83	,1) ,	//Position
									CVektor(0,0	,0	,1),		//Ziel	
									 0,	45,	1,1,0	);	


//Licht
CLicht					g_Licht	(	CVektor(1,1,3)	,	//Position
									0.6			,	//Direktlichtstärke
									0.6			);	//Umgebungslichtstärke

//MatrizenStapel, der die Projektions-, Ansichts- und Objektmatrizen speichert, und zurückgibt, oder Punkte und Normalen in OpenGL aufruft
//Übergeben wird die Adresse der Ansichtsmatrix, die die g_Kamera bei bedarf ermittelt
CMatrizenStapel		MatStapel;	

//Eine Klasse mit vordefinierten Farben
CFarben				Farben;

//Schrift-Objekt
CSchrift	g_Schrift; 

//ein paar Variablen, die von mehreren Funktionen genutzt werden
double ZeitSchritt;
double speed=24*3600;
DWORD start;	
bool Stopp;

void Tastenbelegung();void Rendern (void);double FPS();void ToggleEgo();void ToggleProjOpenGL();

//#########  START: Szene  #############

double rErde=6.368e6*LE, mErde=5.977e24, UmlaufDauerErde=3.156e7, mMond=mErde*1.23e-2;

CSteuerbar Erde1		(CVektor(0,0,2,1), 13, mErde,5e22,"Erde");


void SzeneInitialisieren()
{
//Postitionen der Objekte festlegen 
//(muss hier geschehen, da sich die Positionen ändern wieder zurückgesetzt werden können müssen)
	Erde1.vPosition		= CVektor(0,0,2,1);

//Simulation am Anfang noch stoppen
	Stopp=true;
	speed=24*3600;;

//Zu Zeichnende Objekte für die Objekte der Szene festlegen
	Erde1.Kugel				(20,10,rErde*10);
	
//Start-Geschwindigkeiten festlegen
	Erde1.vGeschwindikkeit=CVektor(-10e-4,0,0);

	Erde1.vKraft.x=+1e17*0;


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

//Gemischte Handlungen, die bei jedem Frame getätigt werden
void Gemischt()
{
	VektorZeichnen(48,CVektor(0,0,0),CVektor(-50,-0,-0),0,"test");

}

//Es kann überprüft werden, ob eine Taste gedrückt wurde ("Ereignis") oder ob sie gehalten wird ("Unten")
void Tastenbelegung()
{
	
//Kamerabewegung
	double faktor=50;

		Erde1.gerade(Anwendung.Taste.Unten['W']);
		Erde1.bremse(Anwendung.Taste.Unten['S']);
		Erde1.links(Anwendung.Taste.Unten['A']);
		Erde1.rechts(Anwendung.Taste.Unten['D']);

		
//Über die Tasten 1-9 hat man Zugriff auf alle Kameras
	for(short i=0;i<9;i++)
		if (Anwendung.Taste.Ereignis [49+i])	Szene.naechsteKamera(i);

//Zeitsteuerung
	if (Anwendung.Taste.Ereignis [VK_ADD])		{speed+=0.5f*speed;}	//Schneller
	if (Anwendung.Taste.Ereignis [VK_SUBTRACT])	{speed-=0.5f*speed;}	//Langsamer
	if (Anwendung.Taste.Ereignis [VK_MULTIPLY])	{speed=-speed;}			//Rückwärts
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
//Normalen der selbst erstellten Flächen anzeigen
	if (Anwendung.Taste.Ereignis [VK_F4])		Anwendung.viewport.normalen_anzeigen=!Anwendung.viewport.normalen_anzeigen;
//Kraft und Geschwindigkeit anzeigen
	if (Anwendung.Taste.Ereignis [VK_F3])		Szene.VektorenAnzeigen=!Szene.VektorenAnzeigen;
//aktive Kamera:
	//Auf Ego-Sicht umschalten (Drehung um eigene Achse, nicht um Ziel)
	if (Anwendung.Taste.Ereignis [VK_F5])		ToggleEgo();
	//Nächstes Objekt der Szene als Ziel wählen
	if (Anwendung.Taste.Ereignis [VK_RBUTTON])		Szene.naechstesZiel();

//Szene nocheinmal von Vorne starten lassen
	if (Anwendung.Taste.Ereignis [VK_F11])		SzeneInitialisieren();

	//if (Anwendung.Taste.Unten	[VK_UP])		(Szene.aktiveKamera())->vor((float)ZeitSchritt*50.0);
	//if (Anwendung.Taste.Unten	[VK_DOWN])		(Szene.aktiveKamera())->vor(-(float)ZeitSchritt*50.0);
	//if (Anwendung.Taste.Unten	[VK_LEFT])		(Szene.aktiveKamera())->rechts(-(float)ZeitSchritt*50.0);
	//if (Anwendung.Taste.Unten	[VK_RIGHT])		(Szene.aktiveKamera())->rechts((float)ZeitSchritt*50.0);

	Anwendung.Taste.EreignisseLeeren();
}

//Informationen, die in jedem Frame eingeblendet werden
void TextInformationen()
{
	
	g_Schrift.Zeile(1,1,0,"FPS:   %.1f",FPS());
	g_Schrift.Zeile(1,2,0,"Zeit:   %.1f",float(GetTickCount()-start)/1000);

	g_Schrift.Zeile(1,4,0,"Kamera:  %d",Szene.aktive_Kamera+1);
	g_Schrift.Zeile(1,5,0,"FOV:   %.1f",(Szene.aktiveKamera())->Oeffnungswinkel);
	g_Schrift.Zeile(1,6,0,"Geschwindigkeit:");
	(Erde1.vGeschwindikkeit*1e6).Schreiben(1,7);
	g_Schrift.Zeile(1,8,0,"Kraft:");
	(Erde1.Kraft).Schreiben(1,9);

	g_Schrift.Zeile(1,11,0,"Speed %.2f T/s",speed/24/3600);

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

//2. Tasten überprüfen
	Tastenbelegung();

//3.  Physikalische Berechnungen
	if(Stopp)
		Szene.PhysikAktualisieren(0);
	else
		Szene.PhysikAktualisieren(ZeitSchritt*speed);

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