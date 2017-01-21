#pragma once

#include "GLInclude.h"

class CTasten 
{		
public:

	CTasten(){};
	
	void EreignisseLeeren(){for(int i=0;i<255;i++)Ereignis [i]=FALSE;};
	void TasteDruecken(WPARAM* wParam,BOOL zustand){Ereignis[*wParam]=zustand;Unten[*wParam]=zustand;};

	BOOL Ereignis [256];
	BOOL Unten [256];
};

struct Application{									// Contains Information Vital To Applications
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
};

struct GL_WindowInit{									// Window Creation Info
	float				FOV;
	float				zNear;
	float				zFar;
	Application*		application;				// Application Structure
	char*				title;						// Window Title
	int					width;						// Width
	int					height;						// Height
	int					bitsPerPixel;				// Bits Per Pixel
	int					resolution; 				// Standard Auflösung für Vollbild
	BOOL				isFullScreen;				// FullScreen?
	BOOL				FullScreenQuestion;			// FullScreen?
	
} ;									// GL_WindowInit

struct GL_Window{									// Contains Information Vital To A Window
	CTasten*			Taste;						// Key Structure
	HWND				hWnd;						// Window Handle
	HDC					hDC;						// Device Context
	HGLRC				hRC;						// Rendering Context
	GL_WindowInit		init;						// Window Init
	BOOL				isVisible;					// Window Visible?
	DWORD				lastTickCount;				// Tick Counter
} ;										// GL_Window

//NeheGL.cpp
void TerminateApplication (GL_Window* window);		// Terminate The Application

void ToggleFullscreen (GL_Window* window);			// Toggle Fullscreen / Windowed Mode

void TogglePolygonMode();							//Johannes Gerer
void ToggleColorMode();								//Johannes Gerer

void ReshapeGL (GL_WindowInit* window);

//Funkionen.cpp
BOOL Initialisierung (GL_Window* window, CTasten *Taste);	// Performs All Your Initialization

void Deinitialisierung (void);							// Performs All Your DeInitialization

void Update (DWORD MilliSekunden);