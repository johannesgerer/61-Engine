//################################
//Einstellungen.h (letzte Änderung (nicht Daten): 10.02.04 00:26)
//################################
/*
In der Funktion "Einstellungen" kann man die Einstellungen bezüglich des Darstellung festlegen.
Die wird von "NeHeGL.cpp" aufgerufen.
TODO: Durch Formular ersetzen.

Mehr Infos:
Die Funktion ohne Rückgabewert, bekommt, von der WinMain (in NeheGL.cpp)
die Klasse des Typs GL_WindowInit übergeben, in die die gewünschten Einstellungen
gespeichert werden können.
*/
int aufloesungen[6][2];

void Einstellungen(GL_WindowInit *einstellung)
{
	aufloesungen[1][0]=640;		aufloesungen[1][1]=480;
	aufloesungen[2][0]=800;		aufloesungen[2][1]=600;
	aufloesungen[3][0]=1024;	aufloesungen[3][1]=768;
	aufloesungen[4][0]=1280;	aufloesungen[4][1]=1024;
	aufloesungen[5][0]=1600;	aufloesungen[5][1]=1200;
	aufloesungen[6][0]=2048;	aufloesungen[6][1]=1536;

	einstellung->resolution			= 5;
	einstellung->FullScreenQuestion	= 0;	//Soll nach Vollbildmodus gefragt werden?
	einstellung->isFullScreen		= 1;
	einstellung->title				= "Johannes' OpenGL Framwork";
	einstellung->width 				= aufloesungen[einstellung->resolution][0];									
	einstellung->height				= aufloesungen[einstellung->resolution][1];
	if(!einstellung->isFullScreen && 0)
	{
	einstellung->width				= 1000;									
	einstellung->height				= 140;	
	}
	einstellung->bitsPerPixel		= 32;									
	einstellung->FOV				= 45.0f;
	einstellung->zNear				= 0.4f;
	einstellung->zFar				= 100.f;
}