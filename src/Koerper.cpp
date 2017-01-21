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

CKoerper::CKoerper( CVektor p, int i_farbe,double i_Masse,double i_c, char* itext, bool i_Physik, bool i_VektorenAnzeigen, bool i_sichtbar, bool i_licht)
:CObjekt(p,i_farbe,itext,0)
{
	VektorenAnzeigen=i_VektorenAnzeigen;
	m_Szene->Hinzu(this);
	Physik=i_Physik;
	Orientierung=MatRot_Y(0);
	Masse=i_Masse;
	vKraft=CVektor(0,0);
	vMoment=vKraft;
	Kraft=CVektor(0,0);
	Moment=Kraft;
	winkelge=Kraft;
	vGeschwindikkeit=vKraft;
	vDrehimpuls=vKraft;
	winkelge=Kraft;
	c=i_c;
}

void CKoerper::Zeichnen(bool Vektoren)
{
	CObjekt::Zeichnen();
	
	if(VektorenAnzeigen || Vektoren)
	{
		VektorZeichnen(10,vPosition,~Kraft*5,0,"F");
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

			F+=r*F_neu;
		}

	vKraft+=F;

	if(c==0)
	{
		vGeschwindikkeit+=(vKraft+Kraft)/Masse/2*sekunden;
		vPosition+= (Kraft/3 + (vKraft)/6)*sekunden*sekunden/Masse+ vGeschwindikkeit * sekunden;
	}else{
		
		double sr_c = sqrt(c);

		for(int i=0;i<3;i++)
		{ 
			if(vKraft(i)==0)
			{
				vPosition.Koordinate(i,sgn(vGeschwindikkeit(i))*log(betrag(vGeschwindikkeit(i)) * sekunden * c/Masse + 1)*Masse / c+vPosition(i));
				vGeschwindikkeit.Koordinate(i, 1 / (sgn(vGeschwindikkeit(i))*c * sekunden /Masse+1/vGeschwindikkeit(i)));

			}else{
				
				double sr_vKraft=sqrt(betrag(vKraft(i)));

				if(sgn(vKraft(i))==sgn(vGeschwindikkeit(i)) || vGeschwindikkeit(i)==0)
				{
					double t7 = Masse / c;
					double t10 = betrag(vGeschwindikkeit(i)) * sr_c;
					double t17 = exp(2 * sekunden * sr_c * sr_vKraft / Masse);
					double t20 = log(((t17 * (sr_vKraft + t10) - t10)/sr_vKraft+1)/2);
					double t25 = -sekunden * sr_vKraft / sr_c + t20 * t7;

					vPosition.Koordinate(i,sgn(vGeschwindikkeit(i))*t25+vPosition(i));
					
					sr_vKraft=sgn(vKraft(i))*sr_vKraft;

					double t5 = vGeschwindikkeit(i) * sr_c;
					double t6 = sr_vKraft + t5;
					double t13 = exp(sgn(vKraft(i))*2 * sekunden * sr_c * sr_vKraft / Masse);
					double t24 = 1 / c * (-2 * sr_c * sr_vKraft + 4 / ( t6 + (sr_vKraft - t5)/t13)  * sr_c * sr_vKraft * t6) / 2;

					vGeschwindikkeit.Koordinate(i, t24);

				}else{
								
					double t11 = sekunden * sr_c * sr_vKraft / Masse;
					double t12 = sin(t11);
					double t15 = cos(t11);
					double t20 = log(sr_c*betrag(vGeschwindikkeit(i))*t12/sr_vKraft + t15)*Masse/c;

					vPosition.Koordinate(i,sgn(vGeschwindikkeit(i))*t20+vPosition(i));

					sr_vKraft=sgn(vKraft(i))*sr_vKraft;

					double t6 = sekunden*sr_c *sr_vKraft/Masse;  
					double t7 = sin(t6);
					double t10 = cos(t6);
					double t21 = sgn(vGeschwindikkeit(i))*(sr_vKraft*t10*betrag(vGeschwindikkeit(i)) - t7 * betrag(vKraft(i)) / sr_c)/(sr_c * t7*betrag(vGeschwindikkeit(i)) + t10 * sr_vKraft);
					
					vGeschwindikkeit.Koordinate(i, t21);
				}
			}

		}
	}

	if(Bremse)
		if(vGeschwindikkeit*Geschwindikkeit<=0)
			vGeschwindikkeit*=0;

	Geschwindikkeit=vGeschwindikkeit;
	Kraft=vKraft;
	vKraft-=F;
	
}

void CSteuerbar::links(bool an)
{ 
	if(!an)
		return; 

	this->paar(CVektor(10,0,0,0),CVektor(0,0.835f*breite,0));
}

void CSteuerbar::rechts(bool an)
{
	if(!an)
		return; 

	this->paar(CVektor(-10,0,0,0),CVektor(0,0.835f*breite,0));
}
void CSteuerbar::gerade(bool an)
{
	if(an)
		this->hinzu(CVektor(1e17,0,0),CVektor()); 
}

void CSteuerbar::bremse(bool an)
{
	Bremse=an;

	if(an)
	{
		this->hinzu(~vGeschwindikkeit*(-5e16),CVektor()); 
		c=5e23;
	}else
		c=5e22;
}