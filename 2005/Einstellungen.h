//################################
//Einstellungen.h (letzte Änderung (nicht Daten): 10.02.04 00:26)
//################################
/*
In der Funktion "Einstellungen" kann man die Einstellungen bezüglich des Darstellung festlegen.
Die wird von "NeHeGL.cpp" aufgerufen.
TODO: Durch Formular ersetzen.

Mehr Infos:
Die Funktion ohne Rückgabewert, bekommt, von der WinMain (in NeheGL.cpp)
die Klasse des Typs AnwendungsDate.übergeben, in die die gewünschten Einstellungen
gespeichert werden können.
*/
#include "cfg.h"
extern AnwendungsDaten Anwendung;

char* Einstellungen()
{
	FILE* fp = fopen("config.txt","r");
	if(fp==0)
		return "config.txt nicht im selben Verzeichnis wie die .exe-Datei!";
	list* parms = read_config(fp);
	fclose(fp);

//####### Auflösung ######
	int aufloesungen[10][2];
	aufloesungen[1][0]=640;		aufloesungen[1][1]=480;
	aufloesungen[2][0]=800;		aufloesungen[2][1]=600;
	aufloesungen[3][0]=1024;	aufloesungen[3][1]=768;
	aufloesungen[4][0]=1152;	aufloesungen[4][1]=864;
	aufloesungen[5][0]=1280;	aufloesungen[5][1]=1024;
	aufloesungen[6][0]=1600;	aufloesungen[6][1]=1200;
	aufloesungen[7][0]=1792;	aufloesungen[7][1]=1344;
	aufloesungen[8][0]=1920;	aufloesungen[8][1]=1440;
	aufloesungen[9][0]=2048;	aufloesungen[9][1]=1536;
	int aufloesung					= atoi(find_value(parms,"aufloesung"))+1;
	if(aufloesung<1 || aufloesung>9)
		return "Fehler in config.txt bei \"aufloesung\"";

	Anwendung.viewport.breite 				= aufloesungen[aufloesung][0];									
	Anwendung.viewport.hoehe				= aufloesungen[aufloesung][1];
//####### Ende Auflösung ######

		Anwendung.startEGO=bool_value(parms,"EGO");
		Anwendung.nachFullScreenFragen=bool_value(parms,"nachFullScreenFragen");
		Anwendung.FullScreen=bool_value(parms,"FullScreen");

		Anwendung.title					= "Johannes' OpenGL Framwork";

	if(!Anwendung.FullScreen && !Anwendung.nachFullScreenFragen &&
	0) //Soll bei Window-Mode diese spezielle Auflösung genommen werden?
	{
		Anwendung.viewport.breite			= 1000;									
		Anwendung.viewport.hoehe		= 140;	
	}

	Anwendung.viewport.bitsProPixel	= atoi(find_value(parms,"bitsProPixel"));									
	Anwendung.viewport.Oeffnungswinkel				= 45.0f;
	Anwendung.viewport.zBildebene				= 0.4;
	Anwendung.viewport.zEnde				= 10e3;
	Anwendung.viewport.ProjOpenGL			= bool_value(parms,"ProjOpenGL");

	Anwendung.viewport.normalen_anzeigen		= bool_value(parms,"normalen_anzeigen");

	return 0;
}
