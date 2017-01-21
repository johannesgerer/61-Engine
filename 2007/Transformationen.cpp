//################################
//Transformationen.h (letzte Änderung: 01.05.04 19:54)
//################################
/*
Diese Datei enthält 
*/


#include "Transformationen.h"
#include "Mathe.h"
#include "NeHeGL.h"

extern Viewport* viewport;

CMat MatProjektion(float Oeffnungswinkel)
{
		glMatrixMode (GL_PROJECTION);	
		glLoadIdentity ();	
	
		CMat ret=CMat();

	float breite_halbe=viewport->zBildebene*tanf(Oeffnungswinkel/360*PI);
	float verhaltnis=(float)viewport->breite/(float)viewport->hoehe;

	if(viewport->ProjOpenGL)
	{
		//gluPerspective(Oeffnungswinkel, verhaltnis,viewport->zBildebene,viewport->zEnde);
		glFrustum(	-breite_halbe				,breite_halbe,
					-breite_halbe/verhaltnis	,breite_halbe/verhaltnis,
					viewport->zBildebene, viewport->zEnde);
		
	

	}else{

		ret= CMat(	1/breite_halbe,	0,							0,							0,
					0,				1/breite_halbe*verhaltnis,	0,							0,
					0,				0,							0,							0,
					0,				0,							-1/viewport->zBildebene,	0	);
	}

		glMatrixMode (GL_MODELVIEW);
		
		glLoadIdentity ();

		return ret; 
}

//Skalierungsmatrix
CMat MatSkal(float sx, float sy, float sz)
{
	return CMat(sx,0,0,0,sy,0,0,0,sz);
}
CMat MatSkal(float s)
{
	return CMat(s,0,0,0,s,0,0,0,s);
}
CMat MatSkal(CVektor s)
{
	return CMat(s.x,0,0,0,s.y,0,0,0,s.z);
}

CMat MatRot_X(double a)
{
	return CMat(1	,0				,0			,
				0	, Cosinus(a)	,-Sinus(a)	,
				0	, Sinus(a)		,Cosinus(a)	);
}

CMat MatRot_Z(double a)
{	
	return CMat(Cosinus(a)	, -Sinus(a)		,0	,
				Sinus(a)	, Cosinus(a)	,0	,
				0			,0				,1	);
}	

CMat MatRot_Y(double a)
{
	return CMat(Cosinus(a)	,0	,-Sinus(a)	,
				0			,1	,0			,
				Sinus(a)	,0	,Cosinus(a)	);
}


CMat MatTrans(CVektor v)
{
	CMat A;
	for(int i=0;i<3;i++)
		A.r[i].w=v(i);
	return A;
}

CMat MatTrans(double x, double y, double z)
{
	return MatTrans(CVektor(x,y,z));
}

CMat MatRot(CVektor start, CVektor achse, double a)
{
	if(a==0)
		return CMat();

	achse=~achse;
	
	CVektor x=achse;

	if(x.x==0 && x.y==0)
		x.x=1;
	x.z+=1;

	CVektor y=~x^achse; 
	

	CMat mat=CMat(achse,y,achse^y);

	return	MatTrans(start)*!mat*MatRot_X(a)*mat*MatTrans(start*(-1));
}

CMat MatRot(CVektor achse, double a)
{
	if(a==0)
		return CMat();
	
		return MatRot(CVektor(), achse,  a);
}