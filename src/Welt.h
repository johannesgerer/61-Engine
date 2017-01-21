#pragma once
#include "Objekt.h"

class CWelt
{
public:
//Konstruktoren:
	CWelt();			//Standardkonstruktor

//Membervaiablen:
	CObjekt* m_Objekte;
	float m_Schwerkraft;

//Funkionen
	void Zeichnen();

};

CWelt::CWelt()
{
	m_Schwerkraft=9.80665F;
	m_Objekte=new CObjekt[];
}