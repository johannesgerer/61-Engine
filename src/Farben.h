//################################
//Farben.h (letzte Änderung (nicht Daten): 25.10.03 23:41)
//################################
/*
Diese Datei deklariert float-Datenfelder,
die RGB-Farbwerte gespeichert haben. 255 enspricht 100%
----------------------------------------------------
Diese Datenfelder werden für die folgende OpenGlFunktion benötigt:
glColor4fv(GLfloat *v). *v soll ein Pointer auf ein float-Datenfeld sein.
Beispiel: glColor4ubv(Rot);
Mit Hilfe dieser Funktion hat man einfachen und 
übersichtlichen Zugriff auf elemtare Farben.
*/

#pragma once

float Rot[4]		=	{ 1.0f, 0.0f, 0.0f, 1.0f};
float WeinRot[4]	=	{ 0.5f, 0.0f, 0.0f, 0.5f};
float Gruen[4]		=	{ 0.0f, 1.0f, 0.0f, 1.0f};
float Blau[4]		=	{ 0.0f, 0.0f, 1.0f, 1.0f};
float Weiss[4]		=	{ 1.0f, 1.0f, 1.0f, 1.0f};
float Schwarz[4]	=	{ 0.0f, 0.0f, 0.0f, 1.0f};
float Rosa[4]		=	{ 1.0f, 0.0f, 1.0f, 1.0f};
float Turkise[4]	=	{ 0.0f, 1.0f, 1.0f, 1.0f};
float Gelb[4]		=	{ 1.0f, 1.0f, 0.0f, 1.0f};
float Orange[4]		=	{ 1.0f, 0.5f, 0.0f, 1.0f};
float GruenBlau[4]	=	{ 0.0f, 1.0f, 0.5f, 1.0f};
float BlauGruen[4]	=	{ 0.0f, 0.5f, 1.0f, 1.0f};
float Glas[4]		=	{ 0.0f, 0.5f, 1.0f, 0.2f};
float Lila[4]		=	{ 0.5f, 0.0f, 1.0f, 1.0f};
float HellGruen[4]	=	{ 0.5f, 1.0f, 0.0f, 1.0f};

float *Farben[]={Rot, WeinRot, Gruen, Blau, Weiss, Schwarz, Rosa, Turkise, Gelb, Orange, GruenBlau, BlauGruen,
				 Glas, Lila, HellGruen};