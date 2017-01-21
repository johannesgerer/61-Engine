//################################
//Licht.h (letzte Änderung: 23.10.03 17:02)
//################################
/*
Klasse CLicht
---------------------------------------
Für die Nutzung von Licht in der Scene:
- Die Lichtwerte werden in dem Konstruktor festgelegt
- Die Funktion "Leuchte" muss für jedes Frame aufgerufen werden, 
	wenn die Möglichkeit besteht, dass sich Werte ändern.
	Ansonsten reicht ein Aufrauf in beim Initialisieren.
*/

#pragma once

#include "Vektor.h"

class CLicht
{
public:

//Membervariablen:
	CVektor m_vPosition;
	float m_pDirekt[4];
	float m_pUmgebung[4];
	bool an;

//Standardkonstruktor
	CLicht(void)
	{
		an=true;
		m_pUmgebung[1]	=	0.1f;
		m_pUmgebung[2]	=	0.1f;
		m_pUmgebung[3]	=	0.1f;
		m_pUmgebung[4]	=	1.0f;
 
		m_pDirekt[1]	=	1.0f;
		m_pDirekt[2]	=	1.0f;
		m_pDirekt[3]	=	1.0f;
		m_pDirekt[4]	=	1;

		m_vPosition = CVektor( -5, 3, -5);
	}

//Memberfunktion
	//Aktiviert und/oder aktualisiert das Licht
	void Leuchte();
};


void CLicht::Leuchte()
{
	float p[4]={m_vPosition.x,m_vPosition.y,m_vPosition.z,1};

	glLightfv( GL_LIGHT1, GL_DIFFUSE,  m_pDirekt );
	glLightfv( GL_LIGHT1, GL_AMBIENT,  m_pUmgebung );
	glLightfv(GL_LIGHT1, GL_POSITION,p);
	glEnable(GL_LIGHT1);
}
