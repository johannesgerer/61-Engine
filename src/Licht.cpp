#include "Licht.h"
#include "MatrizenStapel.h"
#include "Farben.h"
#include "GLInclude.h"

extern CFarben Farben;
extern CMatrizenStapel MatStapel;

CLicht::CLicht(CVektor Position, float iDirekt, float iUmgebung)
{
	an=true;
	Umgebung	=	iUmgebung;
	Direkt		=	iDirekt;
	vPosition	=	Position;
}

void CLicht::Leuchte()
{
	if(!an)
		return;

	CVektor p=MatStapel.Ansicht(vPosition);

	GLfloat umg[]=		{(GLfloat)Umgebung,	(GLfloat)Umgebung,	(GLfloat)Umgebung,	1};
	GLfloat diffuse[]=	{(GLfloat)Direkt,	(GLfloat)Direkt,	(GLfloat)Direkt,	1};
	GLfloat pos[]=		{(GLfloat)p.x,		(GLfloat)p.y,		(GLfloat)p.z,		(GLfloat)p.w};

	glLightfv(	GL_LIGHT1, GL_AMBIENT,umg);
	glLightfv(	GL_LIGHT1, GL_DIFFUSE,diffuse);
	glLightfv(	GL_LIGHT1, GL_POSITION,pos);
	glEnable(	GL_LIGHT1);
}
