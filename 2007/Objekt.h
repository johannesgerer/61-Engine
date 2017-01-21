#pragma once
#include "Vektor.h"
#include "Matrix.h"

class CSzene;
enum Typ;
class CObjekt
	{
	public:
		//Membervariablen:
		static CSzene* m_Szene;		//Pointer auf die Szene des Objekts

		CMat Orientierung;			//Orientierungsmatrix
		CVektor vPosition;			//Position
		CVektor vSkal;				//Dehnung entlang den 3 Achsen

		Typ m_Typ;					//Erscheinungstyp (Definiert in Szene.h)
		int farbe;					//siehe Farben.cpp

		int schichten, seiten;		//Detaileinstellungen für bestimmte Typen (z.B. Kugel)

		char* text;

		float radius, laenge, hoehe, breite;//Eigenschaften der Erscheinung
		bool sichtbar, licht;				//Ist es sichtbar?
											//licht=false: Objekt strahlt eigenes Licht ab
											//licht=true:  Objekt reflektiert Licht, der Umgebung
	
	//Konstruktoren:
		CObjekt(CVektor p, int i_farbe,char* itext=0, bool hinzu=true, bool i_sichtbar=true, bool i_licht=true);
				//Standardkonstruktor, der allgemeine, typ-unabhängige Eigenschaften festlegt

	//Memberfunktionen:
		//-ein Objekt bekommt sein Erscheinungsbild (Initialfunktionen, mit spezifischen Variablen):
			void Kreis		(int i_seiten, float i_radius);
			void Pyramide	(float i_breite, float i_laenge, float i_hoehe);
			void Quader		(float i_breite, float i_laenge, float i_hoehe);
			void Schiff	(float i_breite, float i_laenge, float i_hoehe);
			void R3			(float i_laenge);
			void Kugel		(int i_seiten, int i_schichten, float i_radius);
			void Mine		(float i_laenge);
			
		
		//- Zeichnen
		virtual void Zeichnen();			//Gitb das Objekt in OpenGL aus, das Model hängt vom Typ ab
		
		//- Bewegen:
		void drehen(CVektor achse, double a, CVektor start=CVektor());

};
