#include "Objekt.h"
#include "Mathe.h"
#include "Szene.h"
#include "Farben.h"
#include "Transformationen.h" 
#include "MatrizenStapel.h"
#include "Primitive.h"
#include "NeHeGL.h"
#include "Schrift.h"

extern CSchrift g_Schrift;
extern CFarben Farben;
extern CMatrizenStapel MatStapel;
extern Viewport* viewport;

CObjekt::CObjekt(CVektor p, int i_farbe,char* itext, bool hinzu,bool i_sichtbar, bool i_licht)
	{
		if(hinzu)
			m_Szene->Hinzu(this);
		vPosition=p;
		farbe=i_farbe;
		sichtbar=i_sichtbar;
		licht=i_licht;
		vSkal=CVektor(1);

		text=itext;

		if(itext==0)
			text="";
		
	}


	void CObjekt::Kreis		(int i_seiten, float i_radius)
	{
		m_Typ=tKreis;
		seiten=i_seiten;
		radius=i_radius;
	}

	void CObjekt::Pyramide	(float i_breite, float i_laenge, float i_hoehe)
	{
		m_Typ=tPyramide;

		breite=i_breite;
		laenge=i_laenge;
		hoehe=i_hoehe;
	}

	void CObjekt::Quader	(float i_breite, float i_laenge, float i_hoehe)
	{
		m_Typ=tQuader;
		breite=i_breite;
		laenge=i_laenge;
		hoehe=i_hoehe;
	}

	void CObjekt::Schiff	(float i_breite, float i_laenge, float i_hoehe)
	{
		m_Typ=tSchiff;
		breite=i_breite;
		laenge=i_laenge;
		hoehe=i_hoehe;
	}

	void CObjekt::R3		(float i_laenge)
	{
		m_Typ=tR3;
		laenge=i_laenge;
	}	

	void CObjekt::Kugel		(int i_seiten, int i_schichten, float i_radius)
	{
		m_Typ=tKugel;

		seiten=i_seiten;
		schichten=i_schichten;
		radius=i_radius;
	}

	void CObjekt::Mine		(float i_laenge)
	{
		m_Typ=tMine;
		laenge=i_laenge;
	}	

		void CObjekt::drehen(CVektor achse, double a, CVektor start)
	{
		CMat r=MatRot(start,achse,a);
		Orientierung*=r;
		vPosition*=r;
	}


	
void Wuerfel(int farbe)
{	
	ZeichneWuerfel(1,farbe,1);
}


void CObjekt::Zeichnen()
{
	if(!sichtbar)
		return;

	if(!licht)
		glDisable(GL_LIGHTING);	

	g_Schrift.Schreiben(farbe,vPosition,text);

	MatStapel.neu(MatTrans(vPosition)*Orientierung*MatSkal(vSkal));

	switch(m_Typ)		
	{
	case tKreis:		//###########  Kreis  #############
		{
			
			ZeichneKreis(farbe,seiten,radius);
		}	
		break;
	case tPyramide:		//###########  Pyramide  #############
		{
			MatStapel.hinzu(MatSkal(laenge,breite,hoehe));

			CVektor c[6];
			c[0]=CVektor( 1, 1,0,1)/2;
			c[1]=CVektor(-1, 1,0,1)/2;
			c[2]=CVektor(-1,-1,0,1)/2;
			c[3]=CVektor( 1,-1,0,1)/2;
			c[4]=c[0];
			c[5]=CVektor( 0, 0,1,1);

			glColor4fv(Farben(farbe));
			
			glBegin(GL_QUADS);		//Bodenfläche
			MatStapel.Normale(0,0,-1);
			for(int i=0; i<4;i++)
				MatStapel.Punkt(c[i]);
			glEnd();

			glBegin(GL_TRIANGLES);	//Seitenflächen
			for(int i=0;i<4;i++)
			{
				MatStapel.Normale((c[i]-c[5])^(c[i+1]-c[5]));
				MatStapel.Punkt(c[5]); MatStapel.Punkt(c[i+1]); MatStapel.Punkt(c[i]);
			}
			glEnd();
		}
		break;
	case tSchiff:		//###########  Schiff  #############
		{

		MatStapel.hinzu(MatSkal(laenge,breite,hoehe));

		ZeichneWuerfel(1,farbe,0);

		MatStapel.neu(MatTrans(CVektor(0,0.6f,0)));
		MatStapel.neu(MatSkal(0.2f));
		ZeichneWuerfel(1,farbe,0);
		MatStapel.zurueck();

		MatStapel.hinzu(MatTrans(CVektor(0,0.2f,0)));
		MatStapel.neu(MatSkal(0.5f,0.2f,0.2f));
		ZeichneWuerfel(1,farbe,0);
		MatStapel.zurueck();
		MatStapel.hinzu(MatTrans(CVektor(0.3f,0,0)));
		MatStapel.hinzu(MatSkal(0.1f));
		ZeichneWuerfel(1,farbe,0);
		MatStapel.zurueck();

		MatStapel.hinzu(MatTrans(CVektor(0,-0.6f,0)));
		MatStapel.neu(MatSkal(0.2f));
		ZeichneWuerfel(1,farbe,0);
		MatStapel.zurueck();

		MatStapel.hinzu(MatTrans(CVektor(0,-0.2f,0)));
		MatStapel.neu(MatSkal(0.5f,0.2f,0.2f));
		ZeichneWuerfel(1,farbe,0);
		MatStapel.zurueck();
		MatStapel.hinzu(MatTrans(CVektor(0.3f,0,0)));
		MatStapel.hinzu(MatSkal(0.1f));
		ZeichneWuerfel(1,farbe,1);
	
}
		break;

		case tQuader:		//###########  Quader  #############
		{
		MatStapel.hinzu(MatSkal(laenge,breite,hoehe));

		ZeichneWuerfel(1,farbe);  //Normalen Quader
}
		break;

	case tR3:			//###########  R3  #############
		{
	int f1,f2,f3;
	//wenn die Farbe nicht festgelegt ist, dann rot, grün, blau
	if(farbe==0){f1=11;f2=10;f3=13;}else{f1=farbe;f2=f1;f3=f1;} 

			CVektor p=CVektor();
	
			VektorZeichnen(f2,	p,CVektor(0,laenge,0,0),0,"Y");
			VektorZeichnen(f1,	p,CVektor(laenge,0,0,0),0,"X");
			VektorZeichnen(f3,	p,CVektor(0,0,laenge,0),0,"Z");
		}
		break;
	case tKugel:		//###########  Kugel  #############
		{
			double a[16];
			int f=0;
			for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				a[f]=MatStapel.Szene().r[j](i);
				f++;
			}


			GLUquadricObj *quadric = gluNewQuadric();
			glColor4fv(Farben(farbe));
			glPushMatrix();
			
			glLoadMatrixd(a);
				gluSphere(quadric, radius, seiten, schichten);
			glPopMatrix();
			gluDeleteQuadric(quadric);
		}
		break;
	case tMine:			//###########  Mine  #############
		{
	int f1,f2,f3;
	//wenn die Farbe nicht festgelegt ist, dann rot, grün, blau
	if(farbe==0){f1=11;f2=48;f3=40;}else{f1=farbe;f2=farbe;f3=farbe;} 

	
	MatStapel.hinzu(MatTrans(0,0,1));
	MatStapel.hinzu(MatTrans(-0.5,0.5,0));
	MatStapel.hinzu(MatRot_X(90));
	for (int j=0; j<2; j++)							
		{

			for (int i=0; i<2; i++)						
			{
				ZeichneDreieck(CVektor( 1,-1, 1),CVektor(-1,-1, 1),CVektor( 0, 1, 0),f3,f2,f1,-1);
				//Spiegelung an der Y-Achse 
				MatStapel.hinzu(MatSkal(-1,1,-1));
			}

			//Spiegelung an der Z-Achse
			MatStapel.hinzu(MatSkal(-1,-1,1));
			MatStapel.hinzu(MatRot_Y(90));


		}
	}

		break;
	}

	MatStapel.zurueck();
	glEnable (GL_LIGHTING);	
}
