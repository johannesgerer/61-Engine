#include "Primitive.h"
#include "Mathe.h"
#include "Farben.h"
#include "MatrizenStapel.h"
#include "Transformationen.h" 
#include "NeHeGL.h"

extern CMatrizenStapel MatStapel;
extern CFarben Farben;
extern Viewport* viewport;

void ZeichneKreis(int farbe,int seiten, double radius)
{
	MatStapel.neu(MatSkal(radius));

	glColor4fv(Farben(farbe));
			
			glBegin(GL_LINES);
			MatStapel.Normale(1,0,0);
			float x1=cosf((float)PI*0/(4*seiten));
			float y1=sinf((float)PI*0/(4*seiten));
			float x0=x1;
			float y0=y1;

			for(int i=1; i<=seiten;i++)
			{
				x1=cosf((float)PI*i/(4*seiten));
				y1=sinf((float)PI*i/(4*seiten));

				MatStapel.Punkt(x0,	y0,	0);
				MatStapel.Punkt(x1,	y1,	0);

				MatStapel.Punkt(-x0,	y0,	0);
				MatStapel.Punkt(-x1,	y1,	0);

				MatStapel.Punkt(x0,	-y0,	0);
				MatStapel.Punkt(x1,	-y1,	0);

				MatStapel.Punkt(-x0,	-y0,	0);
				MatStapel.Punkt(-x1,	-y1,	0);

				MatStapel.Punkt(y0,	x0,	0);
				MatStapel.Punkt(y1,	x1,	0);

				MatStapel.Punkt(-y0,	x0,	0);
				MatStapel.Punkt(-y1,	x1,	0);

				MatStapel.Punkt(y0,	-x0,	0);
				MatStapel.Punkt(y1,	-x1,	0);

				MatStapel.Punkt(-y0,	-x0,	0);
				MatStapel.Punkt(-y1,	-x1,	0);

				x0=x1;
				y0=y1;
			}
			glEnd();

			MatStapel.zurueck();

}
void ZeichneWuerfel(float a, int farbe, bool normale)
{ 
	
	MatStapel.neu();
	MatStapel.hinzu(MatSkal(a));

	for(int i2=0;i2<3;i2++)
	{
		if(i2==2)
			MatStapel.hinzu(MatRot_Z(90));
		else
			MatStapel.hinzu(MatRot_Y(90));

		for(int i1=0;i1<2;i1++)
		{
			MatStapel.hinzu(MatSkal(-1,1,1));
			ZeichneQuadrat(1,0.5,0,farbe,1,normale);			
		}
	}
	
	MatStapel.zurueck();

}

void ZeichneQuadrat(float a, float abstand, int xyz, int farbe, int Drehsinn, bool normale)
{
	ZeichneRechteck(a/2,a/2,abstand,xyz,farbe,Drehsinn,normale);
}

void ZeichneDreieck(CVektor p1, CVektor p2, CVektor p3, int farbe1, int farbe2, int farbe3, int Drehsinn, bool normale)
{
glBegin(GL_TRIANGLES);	

	CVektor n=~((p1-p2)^(p1-p3))*Drehsinn;
	
	
	MatStapel.Normale(n);
	

	glColor4fv(Farben(farbe1));
	MatStapel.Punkt(p1);
	glColor4fv(Farben(farbe2));
	MatStapel.Punkt(p2);
	glColor4fv(Farben(farbe3));
	MatStapel.Punkt(p3);

	glEnd();

	//Normale als Strich darstellen
	if(normale || viewport->normalen_anzeigen)
	{
		glDisable(GL_LIGHTING);
			
			glBegin(GL_LINES);
			
				glColor4fv(Farben(17));
				MatStapel.Punkt((p1+p3+p2)/3);
				MatStapel.Punkt((p1+p3+p2)/3+n);
		
			glEnd();

		glEnable(GL_LIGHTING);
	}
}

void ZeichneDreieck(CVektor p1, CVektor p2, CVektor p3, int farbe, int Drehsinn, bool normale)
{
	ZeichneDreieck(p1, p2, p3, farbe, farbe, farbe, Drehsinn, normale);
}


//xyz hat die Werte 0,1,2 für x,y,z
void ZeichneRechteck(float a_halbe, float b_halbe, float abstand, int xyz, int farbe, int Drehsinn, bool normale)
{
	CVektor t=CVektor(0,0,0,1);

	t.Koordinate(xyz,abstand);

	MatStapel.neu(MatTrans(t));

	switch(xyz)
	{
	case 1://y
		MatStapel.hinzu(MatRot_Z(90));
		break;
	case 2://z
		MatStapel.hinzu(MatRot_Y(90));
		break;
	}

	MatStapel.hinzu(MatSkal(1,a_halbe,b_halbe));

	ZeichneVierreck(
	CVektor(0,1,1),CVektor(0,-1,1),
	CVektor(0,-1,-1),CVektor(0,1,-1),
	farbe,Drehsinn,normale);

	MatStapel.zurueck();
}

void ZeichneVierreck(CVektor p1, CVektor p2, CVektor p3, CVektor p4, int farbe, int Drehsinn, bool normale)
{
	glBegin(GL_QUADS);
	
	CVektor n=~((p1-p2)^(p1-p3))*Drehsinn;
	
	glDisable(GL_LIGHTING);
	MatStapel.Normale(n);
	

	glColor4fv(Farben(farbe));

	MatStapel.Punkt(p1);
	MatStapel.Punkt(p2);
	MatStapel.Punkt(p3);
	MatStapel.Punkt(p4);

	glEnd();

	//Normale als Strich darstellen
	if(normale || viewport->normalen_anzeigen)
	{
		glDisable(GL_LIGHTING);
			
			glBegin(GL_LINES);
			
				glColor4fv(Farben(17));
				MatStapel.Punkt((p1+p3+p2+p4)/4);
				MatStapel.Punkt((p1+p3+p2+p4)/4+n);
		
			glEnd();

		glEnable(GL_LIGHTING);
	}

}