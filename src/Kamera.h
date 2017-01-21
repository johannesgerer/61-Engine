#include "Vektor.h"

class CKamera
{
	

public:

	CVektor m_vPosition,m_vZiel,m_vOben;

	CKamera();
	void Schaue();
	void vor(float wert){m_vPosition+=~(m_vZiel-m_vPosition)*wert;};
	void rechts(float wert){m_vPosition.x+=wert;};
	void MausBewegung(GL_WindowInit* Fenster);
};



void CKamera::MausBewegung(GL_WindowInit* Fenster)
{
	POINT mousePos;	
	int MitteX = Fenster->width  >> 1;				// Hälfte der Breite und Hohe
	int MitteY = Fenster->height >> 1;	
	float winkelY=0;
	float winkelX=0;

	GetCursorPos(&mousePos); 
	if((mousePos.x == MitteX) && (mousePos.y == MitteY))
		return;

	SetCursorPos(MitteX, MitteY);

	winkelX = (float)( (MitteX - mousePos.x) ) / 100.0f;		
	winkelY = (float)( (MitteY - mousePos.y) ) / 100.0f;		

	m_vPosition=Rotation(winkelX,	m_vOben,m_vZiel,				m_vPosition);
	m_vPosition=Rotation(winkelY,	(m_vZiel-m_vPosition)^m_vOben,	m_vZiel,m_vPosition);
}


CKamera::CKamera()
{
	m_vPosition	=	CVektor(	-21.01F,	6.0F,	-12.0F);
	m_vZiel		=	CVektor(	0.0F,	0.0F,	0.0F);
	m_vOben		=	CVektor(	0.0F,	1.0F,	0.0F);

}

void CKamera::Schaue()
{
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vZiel.x,	 m_vZiel.y,     m_vZiel.z,	
			  m_vOben.x,	m_vOben.y,		m_vOben.z);
}