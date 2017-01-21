#pragma once
#include "Koerper.h"
#include "Szene.h"
#include "Primitive.h"

class CPlanet :	public CKoerper
{
public:

	CPlanet(int i_farbe, float i_Masse, double r, double i_rBahn, double dauer,int i=1):
	  CKoerper(CVektor(i*i_rBahn*1.496e1,0,0,1), i_farbe, i_Masse*5.977e24)
	  {
		  rBahn=i_rBahn*1.496e1;

		  vGeschwindikkeit=CVektor(0,2*PI/(dauer*3.156e7)*rBahn,0,0);
		  this->m_Typ=tKugel;
		  seiten=10;
		  schichten=10;
		  radius=6.368e-4*r*1000;
	  };

	  void Zeichnen(){
		  CKoerper::Zeichnen(0);
		  //ZeichneKreis(19,10,rBahn);
	  };

	  double rBahn;
};
