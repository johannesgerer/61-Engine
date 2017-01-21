#pragma once
#include "GLInclude.h"

//Diese Datei ist das Bindeglied zwischen NeHeGL.cpp, die Code von http://nehe.gamedev.net enth�lt
//und den von mir geschreibenen .cpp-Dateien (Haupts�chlich "Haupt.cpp")

//Hier werden Funktionen definiert, die von beiden Seiten genutzt werden:

//werden in NeheGL.cpp ausgef�hrt: 
	void TerminateApplication ();	// Terminate The Application
	void ToggleFullscreen ();		// Toggle Fullscreen / Windowed Mode
	void TogglePolygonMode();		//Johannes Gerer
	void ToggleColorMode();			//Johannes Gerer
	void ReshapeGL ();				//Johannes Gerer

//werden in Haupt.cpp ausgef�hrt:
	bool Initialisierung ();			
	
	//Wird von "void ReshapeGL()" in NeHeGL.cpp aufgerufen, wenn sich etwas an der Fenster gr��e ge�ndert aht
	void ProjektionAktualisieren();	
	void Update ();

//Au�erdem werden folgende Klassen und Strukturen definiert:

//1. CTasten beitzt ein Datenfeld mit allen Tasten und deren Zust�nde,
	//welche in NeHeGL.cpp gesetzt und in Haupt.cpp ausgelesen werden.
	class CTasten 
	{		
	public:

		CTasten(){};
		
		void EreignisseLeeren(){for(int i=0;i<255;i++)Ereignis [i]=false;};
		void TasteDruecken(WPARAM* wParam,bool zustand){Ereignis[*wParam]=zustand;Unten[*wParam]=zustand;};
		void TasteDruecken(int VK,bool zustand){Ereignis[VK]=zustand;Unten[VK]=zustand;};

		bool Ereignis [256];
		bool Unten [256];
	};

//2. Enth�lt f�r die Windows API wichtige Informationen, die die Anwendung (hInstace) betreffen
	struct Application{									// Contains Information Vital To Applications
		HINSTANCE		hInstance;						// Application Instance
		const char*		className;						// Application ClassName
	};

//3. Enh�lt die ganzen Handles und Contexts f�r die Windows API 
	struct Window{
		Application*		application;				// Enth�lt Informationen f�r Windows' hInstance
		HWND				hWnd;						// Window Handle
		HDC					hDC;						// Device Context
		HGLRC				hRC;						// Rendering Context
		BOOL				isVisible;					// Window Visible?
	};

//4. Enh�lt Informationen, die den Viewport und die Darstellung der Szene betreffen
	//Diese Informationen werden in Einstellungen.h gesetzt und in vielen der .cpp-Dateien ausgelesen
	struct Viewport{							//Informationen �ber den Viewport
		int					bitsProPixel;				
		int					breite;						
		int					hoehe;						
		float				Oeffnungswinkel;
		float				zBildebene;
		float				zEnde;
		bool				ProjOpenGL;				//Projektion durch
		bool				normalen_anzeigen;
	};

//5. Sammelt alle oben genannten Informationen in einer Struktur, die obige Objekte beinhaltet
	//Davon gibt es im ganzen Programm nur eine Instanz, definiert in "NeHeGL.cpp".
	//Diese wird jedoch ein andere Dateien mit "extern ..." eingebunden
	struct AnwendungsDaten{	
				
		Window				window;				//Enth�lt Informationen f�r Windows' hWnd

		CTasten				Taste;			// Zur Tastaturabfrage	

		Viewport			viewport;		//Informationen �ber den Viewport

		char*				title;			// Window Title

		bool				FullScreen;				// FullScreen?
		bool				nachFullScreenFragen;	// FullScreen?
		bool				startEGO;				// In der Ego sicht starten?
	} ;								