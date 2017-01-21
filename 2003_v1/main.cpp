#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
// This is needed for timeGetTime() (also need to include <mmsystem.h>)
#pragma comment(lib, "winmm.lib")
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

#include "main.h"
#include "vektor.h"										// This includes our header file
#include "Camera.h"
#include "auto.h"

CCamera g_Camera;										// This is our global camera object
CF40 ferrari;
int zeilen;
const float PI=3.1415926535897932384626433832795f;

bool  g_bFullScreen = TRUE;								// Set full screen as default
HWND  g_hWnd;											// This is the handle for the window
RECT  g_rRect;											// This holds the window dimensions
HDC   g_hDC;											// General HDC - (handle to device context)
HGLRC g_hRC;											// General OpenGL_DC - Our Rendering Context for OpenGL
HINSTANCE g_hInstance;


UINT g_FontListID = 0;
HFONT hOldFont;
#define MAX_CHARS	256
#define FONT_HEIGHT 26

UINT CreateOpenGLFont(LPSTR strFontName, int height)	// Build Our Bitmap Font
{
	UINT	fontListID = 0;								// This will hold the base ID for our display list
	HFONT	hFont;										// This will store the handle to our font
	fontListID = glGenLists(MAX_CHARS);					// Generate the list for the font
hFont = CreateFont(	height,							// Our desired HEIGHT of the font
						0,								// The WIDTH (If we leave this zero it will pick the best width depending on the height)
						0,								// The angle of escapement
						0,								// The angle of orientation
						FW_BOLD,						// The font's weight (We want it bold)
						FALSE,							// Italic - We don't want italic
						FALSE,							// Underline - We don't want it underlined
						FALSE,							// Strikeout - We don't want it strikethrough
						ANSI_CHARSET,					// This is the type of character set
						OUT_TT_PRECIS,					// The Output Precision
						CLIP_DEFAULT_PRECIS,			// The Clipping Precision
						ANTIALIASED_QUALITY,			// The quality of the font - We want anitaliased fonts
						FF_DONTCARE|DEFAULT_PITCH,		// The family and pitch of the font.  We don't care.
						strFontName);					// The font name (Like "Arial", "Courier", etc...)
	hOldFont = (HFONT)SelectObject(g_hDC, hFont);
	wglUseFontBitmaps(g_hDC, 0, MAX_CHARS - 1, fontListID);	
	return fontListID;									// Return the ID to the display list to use later
}
void PositionText( int x, int y )
{
	glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	glMatrixMode( GL_PROJECTION );						// Set our matrix to our projection matrix
	glPushMatrix();										// Push on a new matrix to work with
	glLoadIdentity();									// reset the matrix
	glMatrixMode( GL_MODELVIEW );						// Set our matrix to our model view matrix
	glPushMatrix();										// Push on a new matrix to work with
	glLoadIdentity();									// Reset that matrix
	y = SCREEN_HEIGHT - FONT_HEIGHT - y;				// Calculate the weird screen position
	glViewport( x - 1, y - 1, 0, 0 );					// Create a new viewport to draw into
	glRasterPos4f( 0, 0, 0, 1 );						// Set the drawing position
glPopMatrix();										// Pop the current modelview matrix off the stack
	glMatrixMode( GL_PROJECTION );						// Go back into projection mode
	glPopMatrix();										// Pop the projection matrix off the stack

	glPopAttrib();										// This restores our TRANSFORM and VIEWPORT attributes
}
void Init(HWND hWnd)
{
	g_hWnd = hWnd;										// Assign the window handle to a global window handle
	GetClientRect(g_hWnd, &g_rRect);					// Assign the windows rectangle to a global RECT
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);	// Init OpenGL with the global rect
g_FontListID = CreateOpenGLFont("SimSun", FONT_HEIGHT);
						// Position        View		   Up Vector
	g_Camera.PositionCamera(-8, 8.5f, 14,   0, 5, 0,   0, 1, 0);
}
void glDrawText(int x, int y, const char *strString, ...)
{
	char		strText[256];							// This will hold our text to display
	va_list		argumentPtr;							// This will hold the pointer to the argument list
	if (strString == NULL)								// Check if a string was given
		return;											// Don't render anything then
	va_start(argumentPtr, strString);					// Parse the arguments out of the string
	vsprintf(strText, strString, argumentPtr);			// Now add the arguments into the full string
va_end(argumentPtr);								// This resets and frees the pointer to the argument list.
	PositionText(x, y);									// Call our own function to position the text on screen
	glPushAttrib(GL_LIST_BIT);							// This saves the list base information
	glListBase(g_FontListID);							// This sets the lists base
	glCallLists(strlen(strText), GL_UNSIGNED_BYTE, strText);

	glPopAttrib();										// Return the display list back to it's previous state
}
void DestroyFont()										
{
	glDeleteLists(g_FontListID, MAX_CHARS);				// Free the display list
	SelectObject(g_hDC, hOldFont);						// Select the old font back in so we don't have memory leaks
}

///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function Handles the main game loop
/////
///////////////////////////////// MAIN GAME LOOP \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

WPARAM MainLoop()
{
	static bool lines;
	static bool mp;

	MSG msg;

	while(1)											// Do our infinate loop
	{													// Check if there was a message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			if(msg.message == WM_QUIT)					// If the message wasnt to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{ 
			if(GetKeyState('M') & 0x80)
			{
				if(!mp){
					mp=true;
				if(!lines)
				{
					lines=true;
					glPolygonMode(GL_FRONT,GL_LINE);
					glPolygonMode(GL_BACK,GL_LINE);
				}else{
					lines=!true;
					glPolygonMode(GL_FRONT,GL_FILL);
					glPolygonMode(GL_BACK,GL_FILL);
				}}
			}else{
				if(mp)
				mp=false;
			}

			zeilen=0;
			CalculateFrameRate();
			ferrari.update();
			g_Camera.Update();
			RenderScene();
        } 
	}
DestroyFont();	
	DeInit();											// Clean up and free all allocated memory

	return(msg.wParam);									// Return from the program
}


///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This creates a pyramid with the center being (X, Y, Z).
/////   The width and height depend on the WIDTH and HEIGHT passed in
/////
///////////////////////////////// CREATE PYRAMID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreatePyramid(float x, float y, float z, int width, int height)
{
	// Start rendering the 4 triangles for the sides
	glBegin(GL_TRIANGLES);		
		
		// These vertices create the Back Side
		glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);					// Top point
		glColor3ub(255, 255, 255); glVertex3f(x - width, y - height, z - width);	// Bottom left point
		glColor3ub(255, 0, 0); glVertex3f(x + width, y - height, z - width);  // Bottom right point

		// These vertices create the Front Side
		glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);					// Top point
		glColor3ub(255, 255, 255); glVertex3f(x + width, y - height, z + width);  // Bottom right point
		glColor3ub(255, 0, 0); glVertex3f(x - width, y - height, z + width);	// Bottom left point

		// These vertices create the Front Left Side
		glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);					// Top point
		glColor3ub(255, 0, 0); glVertex3f(x - width, y - height, z + width);	// Front bottom point
		glColor3ub(255, 255, 255); glVertex3f(x - width, y - height, z - width);	// Bottom back point

		// These vertices create the Front right Side
		glColor3ub(255, 255, 0);   glVertex3f(x, y + height, z);					// Top point
		glColor3ub(255, 0, 0); glVertex3f(x + width, y - height, z - width);	// Bottom back point
		glColor3ub(255, 255, 255); glVertex3f(x + width, y - height, z + width);	// Front bottom point
			
	glEnd();

	// Now render the bottom of our pyramid

	glBegin(GL_QUADS);

		// These vertices create the bottom of the pyramid
		glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z + width);	// Front left point
		glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z + width);    // Front right point
		glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z - width);    // Back right point
		glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z - width);    // Back left point
	glEnd();
}


///////////////////////////////// DRAW 3D GRID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function create a simple grid to get a better view of the animation
/////
///////////////////////////////// DRAW 3D GRID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Draw3DSGrid()
{
	// Turn the lines GREEN
	glColor3ub(0, 255, 0);

	// Draw a 1x1 grid along the X and Z axis'
	for(float i = -50; i <= 50; i += 1)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

			// Do the horizontal lines (along the X)
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			// Do the vertical lines (along the Z)
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

		// Stop drawing lines
		glEnd();
	}
}


///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This draws a spiral of pyramids to maneuver around
/////
///////////////////////////////// DRAW SPIRAL TOWERS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DrawSpiralTowers()
{							// Create a constant for PI
	const float kIncrease = PI / 16.0f;					// This is the angle we increase by in radians
	const float kMaxRotation = PI * 6;					// This is 1080 degrees of rotation in radians (3 circles)
	float radius = 40;									// We start with a radius of 40 and decrease towards the center

	// Keep looping until we go past the max degrees of rotation (which is 3 full rotations)
	for(float degree = 0; degree < kMaxRotation; degree += kIncrease)
	{
		// Here we use polar coordinates for the rotations, but we swap the y with the z
		float x = float(radius * cos(degree));			// Get the x position for the current rotation and radius
		float z = float(radius * sin(degree));			// Get the z position for the current rotation and radius

		// Create a pyramid for every degree in our spiral with a width of 1 and height of 3 
		CreatePyramid(x, 3, z, 1, 3);
	
		// Decrease the radius by the constant amount so the pyramids spirals towards the center
		radius -= 40 / (kMaxRotation / kIncrease);
	}	
}


///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function renders the entire scene.
/////
///////////////////////////////// RENDER SCENE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void RenderScene() 
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	g_Camera.Look();

	glColor3f(1, 1, 1);zeilen++;
	glDrawText(0, 20, "fps=%d",(int)fps);

	ferrari.render();
	ferrari.textout();
	glLoadIdentity();
	glTranslatef(0,0,0);
	g_Camera.Look();
	Draw3DSGrid();
	//DrawSpiralTowers();

	SwapBuffers(g_hDC);	
}

///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function handles the window messages.
/////
///////////////////////////////// WIN PROC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG    lRet = 0; 
    PAINTSTRUCT    ps;

    switch (uMsg)
	{ 
    case WM_SIZE:										// If the window is resized
		if(!g_bFullScreen)								// Do this only if we are in window mode
		{
			SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));// LoWord=Width, HiWord=Height
			GetClientRect(hWnd, &g_rRect);				// Get the window rectangle
		}
        break; 
 
	case WM_PAINT:										// If we need to repaint the scene
		BeginPaint(hWnd, &ps);							// Init the paint struct		
		EndPaint(hWnd, &ps);							// EndPaint, Clean up
		break;

	case WM_KEYDOWN:

		if(wParam==VK_ESCAPE)
		{
				PostQuitMessage(0);						// Send a QUIT message to the window
				break;
		}

		break;

    case WM_CLOSE:										// If the window is being closes
        PostQuitMessage(0);								// Send a QUIT Message to the window
        break; 
     
    default:											// Return by default
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet;										// Return by default
}

/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Simple Huh!!!  This is probably one of the simplest tutorials in 3D we have.
// Though it might seem complicated to wrap your head around at first, it's
// only a few extra lines of code to add.  Now you guys have no excuse for frame
// based movement in your demos/games :)
// 
// The basic premise is, calculate the time (in seconds) between each frame,
// then multiply that elapsed time by your speed that you desire.  Try it! 
//
// This tutorial includes a new header file, <mmsystem.h>. This is so we can use
// the timeGetTime() function instead of GetTickCount().  Apparently, GetTickCount()
// doesn't seem to update as fast as timeGetTime().  What you really should use
// is QueryPerformanceCounter() and QueryPerformanceFrequency().  These seem to
// be the fastest method to get time from your CPU.
//
// Enjoy!
// 
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
// © 2000-2003 GameTutorials
//
