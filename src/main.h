#ifndef _MAIN_H
#define _MAIN_H


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gl\gl.h>										// Header File For The OpenGL32 Library
#include <gl\glu.h>	
#include "vektor.h"
#include "Camera.h"
#include "auto.h"



/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
// This allows us to use timeGetTime() for our tick count
#include <mmsystem.h>										
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

#define SCREEN_WIDTH 1600									// We want our screen width 800 pixels
#define SCREEN_HEIGHT 1200								// We want our screen height 600 pixels
#define SCREEN_DEPTH 32									// We want 16 bits per pixel




// Extern our global variables so other source files can use them
extern bool  g_bFullScreen;								// Set full screen as default
extern HWND  g_hWnd;									// This is the handle for the window
extern RECT  g_rRect;									// This holds the window dimensions
extern HDC   g_hDC;										// General HDC - (handle to device context)
extern HGLRC g_hRC;										// General OpenGL_DC - Our Rendering Context for OpenGL
extern HINSTANCE g_hInstance;							// This holds our window hInstance
extern float g_FrameInterval;
extern float fps;
extern int zeilen;
extern float g_Time;
extern const float PI;
extern CCamera g_Camera;
extern CF40 ferrari;

// This is our MAIN() for windows
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// The window proc which handles all of window's messages.
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// This controls our main program loop
WPARAM MainLoop();

// This changes the screen to full screen mode
void ChangeToFullScreen();

// This is our own function that makes creating a window modular and easy
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// This allows us to configure our window for OpenGL and backbuffered
bool bSetupPixelFormat(HDC hdc);

// This inits our screen translations and projections
void SizeOpenGLScreen(int width, int height);

// This sets up OpenGL
void InitializeOpenGL(int width, int height);

// This initializes the whole program
void Init(HWND hWnd);

// This draws everything to the screen
void RenderScene();

// This frees all our memory in our program
void DeInit();
void glDrawText(int x, int y, const char *strString, ...);

void CalculateFrameRate();
#endif 


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This tutorial includes a new header file, <mmsystem.h>.  This is so we can use
// the timeGetTime() function instead of GetTickCount().  Apparently, GetTickCount()
// doesn't seem to update as fast as timeGetTime().  If you want to make more
// cross platform code, you can stick with clock(), which is almost as good as
// timeGetTime().
//  
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
