#include "Kamera.h"
#include "Mathe.h"
#include "Transformationen.h"
#include "Szene.h"
#include "Objekt.h"

void CKamera::rechts(float wert)
{
	CVektor d=~(*Ziel-vPosition^vOben)*wert;

	if(m_Ego)
		vPosition+=d;

	vZiel=*Ziel+d;
	Ziel=&vZiel;
	ZielNr=-1;

}
void CKamera::vor(float wert)
{
	if(m_Ego)
	{
	CVektor d=~(*Ziel-vPosition^vOben^vOben)*wert;  
	//um sich auf einer ebene zu bewegen: 
	//statt vPosition: vPosition^vOben^vOben
	//statt +=d; -=d
	vPosition-=d;
	vZiel-=d;
	}else
		vPosition+=~(*Ziel-vPosition)*wert;

}

void CKamera::hoch(float wert)
{
	CVektor d=~(vOben)*wert;

	vZiel=*Ziel+d;
	Ziel=&vZiel;
	ZielNr=-1;

    
	if(m_Ego)	
		vPosition+=d;
}

void CKamera::MausBewegung()
{
	POINT mousePos;	

	GetCursorPos(&mousePos); 
	if((mousePos.x == m_Szene->MitteX) && (mousePos.y == m_Szene->MitteY))
		return;

	float dX = (float)( (m_Szene->MitteX - mousePos.x) )/10;		
	float dY = (float)( (m_Szene->MitteY - mousePos.y) )/10;		


	CVektor vBlick=MatRot(vOben,dX)*(*Ziel-vPosition);

	if(m_Ego)
		vZiel=MatRot(vBlick^vOben,dY)*vBlick+vPosition;
	else
		vPosition=MatRot(vBlick^vOben,dY)*(vBlick*(-1))+*Ziel;

	
	SetCursorPos(m_Szene->MitteX, m_Szene->MitteY);

}

void CKamera::PositionsObjektSetzen(CObjekt* Objekt, CVektor ObjektPosition)
{
	vObjektPosition=ObjektPosition;
	PositionsObjekt=Objekt;
}

CKamera::CKamera(CVektor oben, CVektor Position, CVektor iZiel,
				 bool EGO, float iOeffnungswinkel, bool aktiv, bool ifest, bool istarr)
{
	starr=istarr;
	fest=ifest;
	vZiel=iZiel;
	Ziel=&vZiel;
	ZielNr=-1;
    m_Szene->Hinzu(this,aktiv);
	vOben=oben;
	vPosition=Position;
	Oeffnungswinkel=iOeffnungswinkel;
	m_Ego=EGO;
}

void CKamera::Aktualisieren(bool projektion)
{
	if(projektion)
		Projektion=MatProjektion(Oeffnungswinkel);
	else{

		if(PositionsObjekt!=0)
			vPosition=MatTrans(PositionsObjekt->vPosition)*PositionsObjekt->Orientierung*MatSkal(PositionsObjekt->vSkal)*vObjektPosition;

		if(!starr)
			MausBewegung();

		CVektor rechts,oben,blick;

		blick=~(*Ziel-vPosition);	//Zuständig für die z-Komponente am Bildschirm (in den Bildschirm hinein)
		rechts=~(blick^vOben);		//Zuständig für die y-Komponente am Bildschirm (rechts)
		oben=rechts^blick;			//Zuständig für die x-Komponente am Bildschirm (hoch)
	
		Ansicht=CMat(rechts,oben,blick*(-1))*MatTrans(vPosition*(-1));
	}
}

void CKamera::naechstesZiel()
{
	if(m_Ego)
	{
		vZiel=*Ziel;
		Ziel=&vZiel;
		ZielNr=-1;
	}else{
		ZielNr++;

		if(ZielNr==m_Szene->m_Anzahl+m_Szene->Anzahl_Koerper)
			ZielNr=0;

		if(ZielNr==-1)
			Ziel=&vZiel;
		else
		{	
			if(ZielNr<m_Szene->m_Anzahl)
				Ziel=&m_Szene->m_Objekt[ZielNr]->vPosition;
			else
				Ziel=&m_Szene->m_Koerper[ZielNr-m_Szene->m_Anzahl]->vPosition;
		}
	}
}