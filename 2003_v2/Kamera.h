#include "Vektor.h"

class CKamera
{
	

public:

	CVektor m_vPosition,m_vZiel,m_vOben;

	CKamera();
	void Schaue();
	void vor(float wert){m_vPosition.z+=wert;};
	void rechts(float wert){m_vPosition.x+=wert;};
	void MausBewegung(GL_WindowInit* Fenster);
};

CVektor Rotation(float winkel, CVektor a, CVektor p)
{
	float x=a.x;
	float y=a.y;
	float z=a.z;

	CVektor neu;

	float Cos=cosf(winkel);
	float Sin=sinf(winkel);

	//Siehe http://www.3dsource.de/deutsch/mathe.htm Gleichung 20
	neu.x=	(x*x	*	(1	-	Cos)	+	1	*	Cos)	*	p.x	+
			(x*y	*	(1	-	Cos)	-	z	*	Sin)	*	p.y +
			(x*z	*	(1	-	Cos)	+	y	*	Sin)	*	p.z ;

	neu.y=	(x*y	*	(1	-	Cos)	+	z	*	Sin)	*	p.x	+
			(y*y	*	(1	-	Cos)	+	1	*	Cos)	*	p.y +
			(y*z	*	(1	-	Cos)	-	x	*	Sin)	*	p.z ;

	neu.z=	(x*z	*	(1	-	Cos)	-	y	*	Sin)	*	p.x	+
			(z*y	*	(1	-	Cos)	+	x	*	Sin)	*	p.y +
			(z*z	*	(1	-	Cos)	+	1	*	Cos)	*	p.z ;

    return neu;
}

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
	winkelY = (float)( (MitteY - mousePos.y) ) / 1000.0f;		
	
	m_vPosition=Rotation(winkelX,m_vOben,m_vPosition);
	m_vPosition=Rotation(winkelY,(m_vZiel-m_vPosition)^m_vOben,m_vPosition);

}

CKamera::CKamera()
{
	m_vPosition	=	CVektor(	10.0F,	10.0F,	-5.0F);
	m_vZiel		=	CVektor(	0.0F,	0.0F,	5.0F);
	m_vOben		=	CVektor(	0.0F,	1.0F,	0.0F);
	//m_vPosition	=	CVektor(	0.01F,	6.0F,	15.0F);
	//m_vZiel		=	CVektor(	0.0F,	1.0F,	0.0F);
	//m_vOben		=	CVektor(	0.0F,	1.0F,	0.0F);
}

void CKamera::Schaue()
{
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,	
			  m_vZiel.x,	 m_vZiel.y,     m_vZiel.z,	
			  m_vOben.x,	m_vOben.y,		m_vOben.z);
}