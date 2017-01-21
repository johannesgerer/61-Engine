/***********************************************
*                                              *
*    Jeff Molofee's Revised OpenGL Basecode    *
*  Huge Thanks To Maxwell Sayles & Peter Puck  *
*            http://nehe.gamedev.net           *
*                     2001                     *
*                                              *
***********************************************/

#include <windows.h>													// Header File For The Windows Library

#include "NeHeGL.h"
#include "Einstellungen.h"
#include "Mathe.h"

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif		
#define WM_TOGGLEFULLSCREEN (WM_USER+1)									// Application Define Message For Toggling

static BOOL g_isProgramLooping;											// Window Creation Loop, For FullScreen/Windowed Toggle																		// Between Fullscreen / Windowed Mode
static bool g_createFullScreen;											// If TRUE, Then Create Fullscreen


//#############    Johannes Gerer     ################     Start     ################
static bool PolygonModeLines,ColorMode;

AnwendungsDaten Anwendung;

void ReshapeGL ()
{
	glViewport (0,0, (GLsizei)(Anwendung.viewport.breite), (GLsizei)(Anwendung.viewport.hoehe));
	ProjektionAktualisieren();
}

void TogglePolygonMode()
{
	if(PolygonModeLines)
	{
		glPolygonMode(GL_FRONT,GL_FILL);
		glPolygonMode(GL_BACK,GL_FILL);
	}else
	{
		glPolygonMode(GL_FRONT,GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);
	}
	PolygonModeLines=!PolygonModeLines;
}
void ToggleColorMode()
{
	if(ColorMode)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);

	ColorMode=!ColorMode;
}

//#############    Johannes Gerer     ################     Ende     ################

void TerminateApplication()							// Terminate The Application
{
	PostMessage (Anwendung.window.hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	g_isProgramLooping = FALSE;											// Stop Looping Of The Program
}

BOOL ChangeScreenResolution (Viewport *viewport)	// Change The Screen Resolution
{
	DEVMODE dmScreenSettings;											// Device Mode
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
	dmScreenSettings.dmPelsWidth		= viewport->breite;						// Select Screen Width
	dmScreenSettings.dmPelsHeight		= viewport->hoehe;						// Select Screen Height
	dmScreenSettings.dmBitsPerPel		= viewport->bitsProPixel;					// Select Bits Per Pixel
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// Display Change Failed, Return False
	}
	ReshapeGL();
	return TRUE;														// Display Change Was Successful, Return True
}

BOOL CreateWindowGL ()									// This Code Creates Our OpenGL Window
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

	PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
	{
		sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
		1,																// Version Number
		PFD_DRAW_TO_WINDOW |											// Format Must Support Window
		PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,												// Must Support Double Buffering
		PFD_TYPE_RGBA,													// Request An RGBA Format
		Anwendung.viewport.bitsProPixel,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,												// Color Bits Ignored
		0,																// No Alpha Buffer
		0,																// Shift Bit Ignored
		0,																// No Accumulation Buffer
		0, 0, 0, 0,														// Accumulation Bits Ignored
		16,																// 16Bit Z-Buffer (Depth Buffer)  
		0,																// No Stencil Buffer
		0,																// No Auxiliary Buffer
		PFD_MAIN_PLANE,													// Main Drawing Layer
		0,																// Reserved
		0, 0, 0															// Layer Masks Ignored
	};

	RECT windowRect = {0, 0, Anwendung.viewport.breite, Anwendung.viewport.hoehe};	// Define Our Window Coordinates

	GLuint PixelFormat;													// Will Hold The Selected Pixel Format

	if (Anwendung.FullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
	{
		
		
		if (ChangeScreenResolution (&Anwendung.viewport) == FALSE)
		{
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			Anwendung.FullScreen = FALSE;							// Set FullScreen To False (Windowed Mode)
		}
		else															// Otherwise (If Fullscreen Mode Was Successful)
		{
			ShowCursor (FALSE);											// Turn Off The Cursor
			windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
			windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
		}																// (Top Window Covering Everything Else)
	}
	else																// If Fullscreen Was Not Selected
	{
		// Adjust Window, Account For Window Borders
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// Create The OpenGL Window
	Anwendung.window.hWnd = CreateWindowEx (windowExtendedStyle,					// Extended Style
								   Anwendung.window.application->className,	// Class Name
								   Anwendung.title,					// Window Title
								   windowStyle,							// Window Style
								   0, 0,								// Window X,Y Position
								   windowRect.right - windowRect.left,	// Window Width
								   windowRect.bottom - windowRect.top,	// Window Height
								   HWND_DESKTOP,						// Desktop Is Window's Parent
								   0,									// No Menu
								   Anwendung.window.application->hInstance, // Pass The Window Instance
								   &Anwendung);

	if (Anwendung.window.hWnd == 0)												// Was Window Creation A Success?
	{
		return FALSE;													// If Not Return False
	}

	Anwendung.window.hDC = GetDC (Anwendung.window.hWnd);									// Grab A Device Context For This Window
	if (Anwendung.window.hDC == 0)												// Did We Get A Device Context?
	{
		// Failed
		DestroyWindow (Anwendung.window.hWnd);									// Destroy The Window
		Anwendung.window.hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	PixelFormat = ChoosePixelFormat (Anwendung.window.hDC, &pfd);				// Find A Compatible Pixel Format
	if (PixelFormat == 0)												// Did We Find A Compatible Format?
	{
		// Failed
		ReleaseDC (Anwendung.window.hWnd, Anwendung.window.hDC);							// Release Our Device Context
		Anwendung.window.hDC = 0;												// Zero The Device Context
		DestroyWindow (Anwendung.window.hWnd);									// Destroy The Window
		Anwendung.window.hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	if (SetPixelFormat (Anwendung.window.hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
	{
		// Failed
		ReleaseDC (Anwendung.window.hWnd, Anwendung.window.hDC);							// Release Our Device Context
		Anwendung.window.hDC = 0;												// Zero The Device Context
		DestroyWindow (Anwendung.window.hWnd);									// Destroy The Window
		Anwendung.window.hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	Anwendung.window.hRC = wglCreateContext (Anwendung.window.hDC);						// Try To Get A Rendering Context
	if (Anwendung.window.hRC == 0)												// Did We Get A Rendering Context?
	{
		// Failed
		ReleaseDC (Anwendung.window.hWnd, Anwendung.window.hDC);							// Release Our Device Context
		Anwendung.window.hDC = 0;												// Zero The Device Context
		DestroyWindow (Anwendung.window.hWnd);									// Destroy The Window
		Anwendung.window.hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	// Make The Rendering Context Our Current Rendering Context
	if (wglMakeCurrent (Anwendung.window.hDC, Anwendung.window.hRC) == FALSE)
	{
		// Failed
		wglDeleteContext (Anwendung.window.hRC);									// Delete The Rendering Context
		Anwendung.window.hRC = 0;												// Zero The Rendering Context
		ReleaseDC (Anwendung.window.hWnd, Anwendung.window.hDC);							// Release Our Device Context
		Anwendung.window.hDC = 0;												// Zero The Device Context
		DestroyWindow (Anwendung.window.hWnd);									// Destroy The Window
		Anwendung.window.hWnd = 0;												// Zero The Window Handle
		return FALSE;													// Return False
	}

	ShowWindow (Anwendung.window.hWnd, SW_NORMAL);								// Make The Window Visible
	Anwendung.window.isVisible = TRUE;											// Set isVisible To True

	ReshapeGL ();				// Reshape Our GL Window

	ZeroMemory (&Anwendung.Taste, sizeof (CTasten));						// Clear All Keys

	return TRUE;														// Window Creating Was A Success
																		// Initialization Will Be Done In WM_CREATE
}

BOOL DestroyWindowGL ()								// Destroy The OpenGL Window & Release Resources
{
	if (Anwendung.window.hWnd != 0)												// Does The Window Have A Handle?
	{	
		if (Anwendung.window.hDC != 0)											// Does The Window Have A Device Context?
		{
			wglMakeCurrent (Anwendung.window.hDC, 0);							// Set The Current Active Rendering Context To Zero
			if (Anwendung.window.hRC != 0)										// Does The Window Have A Rendering Context?
			{
				wglDeleteContext (Anwendung.window.hRC);							// Release The Rendering Context
				Anwendung.window.hRC = 0;										// Zero The Rendering Context

			}
			ReleaseDC (Anwendung.window.hWnd, Anwendung.window.hDC);						// Release The Device Context
			Anwendung.window.hDC = 0;											// Zero The Device Context
		}
		DestroyWindow (Anwendung.window.hWnd);									// Destroy The Window
		Anwendung.window.hWnd = 0;												// Zero The Window Handle
	}

	if (Anwendung.FullScreen)										// Is Window In Fullscreen Mode
	{
		ChangeDisplaySettings (NULL,0);									// Switch Back To Desktop Resolution
		ShowCursor (TRUE);												// Show The Cursor
	}	
	return TRUE;														// Return True
}

// Process Window Message Callbacks
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//#######################################################
	// Get The Window Context
	AnwendungsDaten* window = (AnwendungsDaten*)(GetWindowLong (hWnd, GWL_USERDATA));
	//#######################################################
	//Achtung: Unter Projekteigenschaften/Linker/System
	//				 Subsystem: Windows (/SUBSYSTEM:WINDOWS)
	//Johannes Gerer


	switch (uMsg)														// Evaluate Anwendung Message
	{
		case WM_SYSCOMMAND:												// Intercept System Commands
		{
			switch (wParam)												// Check System Calls
			{
				case SC_SCREENSAVE:										// Screensaver Trying To Start?
				case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
				return 0;												// Prevent From Happening
			}
			break;														// Exit
		}
		return 0;														// Return

		case WM_CREATE:													// Anwendung Creation
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Anwendung Structure Pointer
			window = (AnwendungsDaten*)(creation->lpCreateParams);
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														// Return

		case WM_CLOSE:													// Closing The Window
			TerminateApplication();								// Terminate The Application
		return 0;														// Return

		case WM_SIZE:												// Size Action Has Taken Place
			switch (wParam)												// Evaluate Size Action
			{
				case SIZE_MINIMIZED:									// Was Window Minimized?
					window->window.isVisible = FALSE;							// Set isVisible To False
				return 0;												// Return

				case SIZE_MAXIMIZED:									// Was Window Maximized?
					window->window.isVisible = TRUE;
					window->viewport.breite=LOWORD (lParam);
					window->viewport.hoehe=HIWORD (lParam);
					ReshapeGL ();
				return 0;												// Return

				case SIZE_RESTORED:										// Was Window Restored?
					window->window.isVisible = TRUE;							// Set isVisible To True
					window->viewport.breite=LOWORD (lParam);
					window->viewport.hoehe=HIWORD (lParam);
					ReshapeGL ();
				return 0;												// Return
			}
		break;															// Break

		case WM_LBUTTONDOWN:
			window->Taste.TasteDruecken(VK_LBUTTON,TRUE);
			break;

		case WM_RBUTTONDOWN:
			window->Taste.TasteDruecken(VK_RBUTTON,TRUE);
			break;

		case WM_KEYDOWN:					// Update Keyboard Buffers For Keys Pressed
			if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
			{
				window->Taste.TasteDruecken(&wParam,TRUE);
				
				return 0;												// Return
			}
		break;															// Break

		case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
				if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
			{
				window->Taste.TasteDruecken(&wParam,FALSE);
				return 0;												// Return
			}
		break;															// Break

		case WM_TOGGLEFULLSCREEN:										// Toggle FullScreen Mode On/Off
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage (hWnd, WM_QUIT, 0, 0);
		break;															// Break
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
}

BOOL RegisterWindowClass (Application* application)						// Register A Window Class For This Application.
{																		// TRUE If Successful
	// Register A Window Class
	WNDCLASSEX windowClass;												// Window Class
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WindowProc Handles Messages
	windowClass.hInstance		= application->hInstance;				// Set The Instance
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// Class Background Brush Color
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	windowClass.lpszClassName	= application->className;				// Sets The Applications Classname
	if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
	{
		// NOTE: Failure, Should Never Happen
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// Return False (Failure)
	}
	return TRUE;														// Return True (Success)
}

// Program Entry (WinMain)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	Application			application;									// Application Structure
	BOOL				isMessagePumpActive;							// Message Pump Active?
	MSG					msg;											// Window Message Structure
	
	// Fill Out Application Data
	application.className = "OpenGL";									// Application Class Name
	application.hInstance = hInstance;									// Application Instance

	// Fill Out Window
	ZeroMemory (&Anwendung, sizeof (AnwendungsDaten));							// Make Sure Memory Is Zeroed
	Anwendung.window.application	= &application;							// Anwendung Application

	char* Fehler=Einstellungen();
	if(Fehler!=0)//Johannes Gerer
	{
		MessageBox (HWND_DESKTOP, Fehler, "Fehler", MB_OK | MB_ICONEXCLAMATION);
		return -1; 
	}

	ZeroMemory (&Anwendung.Taste, sizeof (CTasten));									// Zero keys Structure

	// Ask The User If They Want To Start In FullScreen Mode?
	if(Anwendung.nachFullScreenFragen)
		if (MessageBox (HWND_DESKTOP, "Vollbild ?", "Vollbild ?", MB_YESNO | MB_ICONQUESTION) == IDNO)
			Anwendung.FullScreen = FALSE;								// If Not, Run In Windowed Mode
		else
			Anwendung.FullScreen = TRUE;

	// Register A Class For Our Anwendung To Use
	if (RegisterWindowClass (&application) == FALSE)					// Did Registering A Class Fail?
	{
		// Failure
		MessageBox (HWND_DESKTOP, "Error Registering Anwendung Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// Terminate Application
	}

	g_isProgramLooping = TRUE;											// Program Looping Is Set To TRUE
	g_createFullScreen = Anwendung.FullScreen;						// g_createFullScreen Is Set To User Default
	while (g_isProgramLooping)											// Loop Until WM_QUIT Is Received
	{
		// Create A Anwendung
		Anwendung.FullScreen = g_createFullScreen;					// Set Init Param Of Anwendung Creation To Fullscreen?
		if (CreateWindowGL () == TRUE)							// Was Anwendung Creation Successful?
		{
			// At This Point We Should Have A Anwendung That Is Setup To Render OpenGL
			if (Initialisierung() == FALSE)					// Call User Intialization
			{
				// Failurebool
				TerminateApplication ();							// Close Anwendung, This Will Handle The Shutdown
			}
			else														// Otherwise (Start The Message Pump)
			{	// Initialize was a success
				isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE
				while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
				{
					// Success Creating Anwendung.  Check For Anwendung Messages
					if (PeekMessage (&msg, Anwendung.window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// Check For WM_QUIT Message
						if (msg.message != WM_QUIT)// && msg.m		essage!=WM_PAINT && msg.message!=WM_MOUSEMOVE)						// Is The Message A WM_QUIT Message?
						{
							DispatchMessage (&msg);						// If Not, Dispatch The Message
					
						}
						else											// Otherwise (If Message Is WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// Terminate The Message Pump
						}
					}
					else												// If There Are No Messages
					{
						if (Anwendung.window.isVisible == FALSE)					// If Anwendung Is Not Visible
						{
							WaitMessage ();								// Application Is Minimized Wait For A Message
						}
						else											// If Anwendung Is Visible
						{
							Update ();	// Update The Scene
							SwapBuffers (Anwendung.window.hDC);					// Swap Buffers (Double Buffering)
						}
					}
				}														// Loop While isMessagePumpActive == TRUE
			}															// If (Initialize (...

			// Application Is Finished
		
			DestroyWindowGL ();									// Destroy The Active Anwendung
		}
		else															// If Anwendung Creation Failed
		{
			// Error Creating Anwendung
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Anwendung", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// Terminate The Loop
		}
	}																	// While (isProgramLooping)

	UnregisterClass (application.className, application.hInstance);		// UnRegister Anwendung Class
	return 0;
}																		// End Of WinMain()
