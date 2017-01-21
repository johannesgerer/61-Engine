#include "koerper.h"
#include "Transformationen.h"
#include "Szene.h"
#include "Vektor.h"
#include "Mathe.h"

double CKoerper::InitEnergie()
{
	E_kin_0=E_kin();
	E_pot_0=E_pot();

	return E_kin_0+E_pot_0;
}

double CKoerper::E_kin()
{
	return 0.5*Masse*vGeschwindikkeit.sqBetrag();
}

double CKoerper::E_pot()
{
	double E=0;

for(int i=0;i<m_Szene->Anzahl_Koerper;i++)//Gravitationskräfte zwischem aktuellem und allen anderen Körpern
	if(this!=m_Szene->m_Koerper[i])
		if(m_Szene->m_Koerper[i]->Masse!=0 && m_Szene->m_Koerper[i]->Physik)
		{
			CVektor r=m_Szene->m_Koerper[i]->vPosition-vPosition;
			E-=m_Szene->m_Koerper[i]->Masse*Masse*G*(1/!(vPosition-m_Szene->m_Koerper[i]->vPosition));
		}

	return E/2;
}

CKoerper::CKoerper(CVektor p, int i_farbe,double i_Masse, char* itext, bool i_Physik, bool i_VektorenAnzeigen, bool i_sichtbar, bool i_licht)
:CObjekt(p,i_farbe,itext,0)
{
	VektorenAnzeigen=i_VektorenAnzeigen;
	m_Szene->Hinzu(this);
	Physik=i_Physik;
	Orientierung=MatRot_Y(360);
	Masse=i_Masse;
	vKraft=CVektor(0,0);
	vMoment=vKraft;
	Kraft=CVektor(0,0);
	Moment=Kraft;
	winkelge=Kraft;
	vGeschwindikkeit=vKraft;
	vDrehimpuls=vKraft;
	winkelge=Kraft;
}

void CKoerper::Zeichnen(bool Vektoren)
{
	CObjekt::Zeichnen();
	
	if(VektorenAnzeigen || Vektoren)
	{
		VektorZeichnen(10,vPosition,Kraft/sqrt(!Kraft)*LE*1.5e1,0,"F");
		VektorZeichnen(11,vPosition,vGeschwindikkeit*10e4,0,"v");
	}
}

void CKoerper::hinzu(CVektor F, CVektor r, bool ObjektKoordinaten)
{	
	if(ObjektKoordinaten)
	{
		F=Orientierung*F;
		r=Orientierung*r+vPosition;
	}		
	
	vMoment+=F^(r-vPosition);
	F.w=0;
	vKraft+=F;

}

//Kräftepaar
void CKoerper::paar(CVektor F, CVektor r, bool ObjektKoordinaten)
{	
	this->hinzu(F*2,r,ObjektKoordinaten);
	if(ObjektKoordinaten)
	this->hinzu(F*(-2),CVektor(),1);
	else
		this->hinzu(F*(-2),vPosition,0);
}

void CKoerper::PhysikAktualisieren(double sekunden)
{
//Traegheitstensor=!MatSkal(CVektor(hoehe*laenge,hoehe*breite,laenge*breite)*Masse/12);
	if(!Physik)
		return;

double F_neu;

for(int i=0;i<m_Szene->Anzahl_Koerper;i++)//Gravitationskräfte zwischem aktuellem und allen anderen Körpern
	if(this!=m_Szene->m_Koerper[i])
		if(m_Szene->m_Koerper[i]->Masse!=0 && m_Szene->m_Koerper[i]->Physik)
		{
			CVektor r=m_Szene->m_Koerper[i]->vPosition-vPosition; //Verbindungsvektor
			//if(!r>0.001f)
			F_neu=G*m_Szene->m_Koerper[i]->Masse*Masse/hoch(!r,3);

			vKraft+=r*F_neu;
		}

		//if(sekunden!=0)
		//	vKraft*=1+(F_neu-F)/sekunden/!vKraft;

		
		vGeschwindikkeit+=(vKraft+Kraft)/Masse/2*sekunden;
		vPosition+=vKraft/Masse*sekunden*sekunden*0.5+vGeschwindikkeit*sekunden;

//		Orientierung*=MatRot(winkelge,!winkelge*sekunden);
	
	
	//vGeschwindikkeit+=vKraft/Masse*sekunden;	

	//if(!vDrehimpuls!=0)
	{
	//winkelge=Orientierung*Traegheitstensor*(!Orientierung)*vDrehimpuls;

	//Orientierung*=MatRot(winkelge,(!winkelge)*sekunden*180/PI);
	}

	//vDrehimpuls+=vMoment*sekunden;

	Geschwindikkeit=vGeschwindikkeit;
	Kraft=vKraft;
	Moment=vMoment;

	vKraft=CVektor(0,0);
	vMoment=vKraft;
}

void CSteuerbar::links()
{
	this->paar(CVektor(10,0,0,0),CVektor(0,0.835f*breite,0));
}

void CSteuerbar::rechts()
{
	this->paar(CVektor(-10,0,0,0),CVektor(0,0.835f*breite,0));
}
void CSteuerbar::gerade()
{
	this->hinzu(CVektor(300,0,0),CVektor());
}
void CSteuerbar::bremse()
{
	if(!vDrehimpuls<1)
		vDrehimpuls=CVektor(0,0);
	else
		vDrehimpuls*=1-0.05f;

	if(!vGeschwindikkeit<1)
		vGeschwindikkeit=CVektor(0,0);
	else
		this->hinzu(~vGeschwindikkeit*(-50),vPosition,0);
}
