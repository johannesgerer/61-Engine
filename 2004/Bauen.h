//################################
//Bauen.h (letzte Änderung: 26.10.03 23:41)
//################################
//Das ist eine vorläufige Klasse, die die Welt zeichnet,
//bis die Klasse CWelt ihr eigenen Modell laden und zeichnen kann.

#pragma once
#include "vektor.h"

class CBauen
{
public:
	CBauen(){};
	void Los(){Draw3DSGrid();};

//private:
	float m_angle;
	void angle(float a){m_angle+=a;};
	void DrawMine(CVektor pos);
	void Draw3DSGrid();
	void CreatePyramid(float x, float y, float z, int width, int height);
	void DrawSpiral();
	void Flaechen(CVektor p);
	void Kugeln(float o, float h, float l,CVektor kreuz, float x);
	void Dreieck(CVektor* a, CVektor* b, CVektor* c,float* farbe=Glas);
	void KugelZeichnen(float r, CVektor *p, float* farbe,int s1, int s2);
	void CBauen::KreisZeichnen(float r, CVektor *p, CVektor *n, float* farbe,int s);
};

void CBauen::Dreieck(CVektor* a, CVektor* b, CVektor* c,float* farbe)
{
//	CVektor n=(*a-*c)^(*a-*b);
	g_Schrift.Schreiben(Weiss,*a,"A");
	g_Schrift.Schreiben(Weiss,*b,"B");
	g_Schrift.Schreiben(Weiss,*c,"C");

	glBegin(GL_TRIANGLES);	
		//glNormal3fv(n.GLausgabe(5));
		glColor4fv(farbe);  glVertex3fv(a->GLausgabe());
		glColor4fv(farbe);	glVertex3fv(b->GLausgabe());
		glColor4fv(farbe);	glVertex3fv(c->GLausgabe());
	glEnd();
}

void CBauen::Kugeln(float o, float h, float l, CVektor kreuz, float x)
{
	GLUquadricObj *pSphere = gluNewQuadric();
	
	glPushMatrix();
	glRotatef(acosf(x)*180/PI,kreuz.x,kreuz.y,kreuz.z);
	glRotatef(o,1,0,0);
	for(int i=0; i<4; i++)
	{
		glPushMatrix();
		glRotatef(90*i,1,0,0);
		glTranslatef(h,l+0.5,0);
		glRotatef(2.14*o,1,0,0);
		glColor4fv(Farben[i+3]);
		gluSphere(pSphere, 0.2, 20, 20);
		glPopMatrix();
	}
	glPopMatrix();
	
	gluDeleteQuadric(pSphere);
}

void CBauen::KugelZeichnen(float r, CVektor *p, float* farbe,int s1, int s2)
{
	GLUquadricObj *pSphere = gluNewQuadric();
	glPushMatrix();
	glTranslatef(p->x,p->y,p->z);
	glColor4fv(farbe);
	gluSphere(pSphere, r, s1, s2);
	glPopMatrix();
	gluDeleteQuadric(pSphere);
}
void KreisPunkt(float x0,float y0,float x1,float y1)
{
glVertex3f(x0,	y0,	0);
glVertex3f(x1,	y1,	0);

glVertex3f(-x0,	y0,	0);
glVertex3f(-x1,	y1,	0);

glVertex3f(x0,	-y0,	0);
glVertex3f(x1,	-y1,	0);

glVertex3f(-x0,	-y0,	0);
glVertex3f(-x1,	-y1,	0);

glVertex3f(y0,	x0,	0);
glVertex3f(y1,	x1,	0);

glVertex3f(-y0,	x0,	0);
glVertex3f(-y1,	x1,	0);

glVertex3f(y0,	-x0,	0);
glVertex3f(y1,	-x1,	0);

glVertex3f(-y0,	-x0,	0);
glVertex3f(-y1,	-x1,	0);
}
void CBauen::KreisZeichnen(float r, CVektor *p, CVektor *n, float* farbe,int s)
{
	CVektor ro=~(*n^CVektor(0,0,1))*5;		//Vektor um den Rotiert werden soll Normale x Z-Achse

	glPushMatrix();
	glTranslatef(p->x,p->y,p->z);			//Mittelpunkt im Raum versetzen	
	glRotatef(								//Rotieren um Vektor ro
			180-acosf((*n*CVektor(0,0,1))/!(*n))*180/PI,ro.x,ro.y,ro.z);

	glColor4fv(farbe);
	glBegin(GL_LINES);

	float x2=cosf(PI*0/(4*s))*r;
	float y2=sinf(PI*0/(4*s))*r;
	float x1=x2;
	float y1=y2;

	for(int i=1; i<=s;i++)
	{
		x2=cosf(PI*i/(4*s))*r;
		y2=sinf(PI*i/(4*s))*r;
		KreisPunkt(x1,y1,x2,y2);
		x1=x2;
		y1=y2;
	}
	glEnd();

	glPopMatrix();
}

void CBauen::Flaechen(CVektor p)
{
	KugelZeichnen(0.2,&p,Gelb,10,10);

	glColor4f(1,1,1,0.5f);
glBegin(GL_QUADS);
glNormal3f( 0.0f, 1.0f, 0.0f);
glColor4fv(WeinRot); glVertex3f(50,-0.1f, -50);
glColor4fv(WeinRot); glVertex3f(-50,-0.1f, -50);
glColor4fv(WeinRot); glVertex3f(-50,-0.1f, 50);
glColor4fv(WeinRot); glVertex3f(50,-0.1f, 50);

glColor4f(1,1,1,1);
glNormal3f( 0.0f, 0.0f, -1.0f);
	glColor4fv(Glas); glVertex3f(0, 0, 6);
	glColor4fv(Glas); glVertex3f(7, 0, 6);
	glColor4fv(Glas); glVertex3f(7, 7, 6);
	glColor4fv(Glas); glVertex3f(0, 7, 6);
	glEnd();
}

void CBauen::DrawMine(CVektor pos)
{
	glPushMatrix();
	glTranslatef (pos.x,pos.y,pos.z);
	glRotatef (m_angle, 0.0f, 1.0f, 0.0f);						// Rotate On The Y-Axis By angle
	

	for (int j=0; j<2; j++)								// 2 Passes
	{
		glRotatef(90.0f,0.0f,1.0f,0.0f);						// Rotate 90 Degrees On The Y-Axis
		glRotatef(180.0f,1.0f,0.0f,0.0f);						// Rotate 180 Degress On The X-Axis
		for (int i=0; i<2; i++)							// 2 Passes
		{
			glRotatef(180.0f,0.0f,1.0f,0.0f);					// Rotate 180 Degrees On The Y-Axis
			glBegin (GL_TRIANGLES);								// Begin Drawing Triangles
				glNormal3f( 0.0f, 0.0f, 1.0f);
				glColor3fv(HellGruen);	glVertex3f( 0.0f, 1.0f, 0.0f);
				glColor3fv(Orange);	glVertex3f(-1.0f,-1.0f, 1.0f);
				glColor3fv(Lila);	glVertex3f( 1.0f,-1.0f, 1.0f);
			glEnd ();											// Done Drawing Triangles
		}
	}
glPopMatrix();
}

void CBauen::DrawSpiral()
{							// Create a constant for PI
	float PI = 3.14f;
	const float kIncrease = PI / 16.0f;					// This is the angle we increase by in radians
	const float kMaxRotation = PI * 6;					// This is 1080 degrees of rotation in radians (3 circles)
	float radius = 40;									// We start with a radius of 40 and decrease towards the center

	// Keep looping until we go past the max degrees of rotation (which is 3 full rotations)
	for(float degree = 0; degree < kMaxRotation; degree += kIncrease)
	{
		// Here we use polar coordinates for the rotations, but we swap the y with the z
		float x = float(radius * cosf(degree));			// Get the x position for the current rotation and radius
		float z = float(radius * sinf(degree));			// Get the z position for the current rotation and radius

		// Create a pyramid for every degree in our spiral with a width of 1 and height of 3 
		if(0)
			CreatePyramid(x, 3, z, 1, 3);
		else
			DrawMine(CVektor(x,1,z));
	
		// Decrease the radius by the constant amount so the pyramids spirals towards the center
		radius -= 40 / (kMaxRotation / kIncrease);
	}	
}
void CBauen::Draw3DSGrid()
{
	// Turn the lines GREEN
	glColor3fv(Gruen);
	glNormal3f( 0.0f, 1.0f, 0.0f);
	// Draw a 1x1 grid along the X and Z axis'
	for(float i = -50; i <= 50; i += 1)
	{
		// Start drawing some lines
		glBegin(GL_LINES);

			// Do the horizontal lines (along the X)
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			// Do the vertical lines (along the Z)
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

		// Stop drawing lines
		glEnd();
	}
}
void CBauen::CreatePyramid(float x, float y, float z, int width, int height)
{
	// Start rendering the 4 triangles for the sides
	glBegin(GL_TRIANGLES);		
		
		// These vertices create the Back Side
		glColor3fv(Gelb);   glVertex3f(x, y + height, z);					// Top point
		glColor3fv(Weiss); glVertex3f(x - width, y - height, z - width);	// Bottom left point
		glColor3fv(Rot); glVertex3f(x + width, y - height, z - width);  // Bottom right point

		// These vertices create the Front Side
		glColor3fv(Gelb);   glVertex3f(x, y + height, z);					// Top point
		glColor3fv(Weiss); glVertex3f(x + width, y - height, z + width);  // Bottom right point
		glColor3fv(Rot); glVertex3f(x - width, y - height, z + width);	// Bottom left point

		// These vertices create the Front Left Side
		glColor3fv(Gelb);   glVertex3f(x, y + height, z);					// Top point
		glColor3fv(Rot); glVertex3f(x - width, y - height, z + width);	// Front bottom point
		glColor3fv(Weiss); glVertex3f(x - width, y - height, z - width);	// Bottom back point

		// These vertices create the Front right Side
		glColor3fv(Gelb);   glVertex3f(x, y + height, z);					// Top point
		glColor3fv(Rot); glVertex3f(x + width, y - height, z - width);	// Bottom back point
		glColor3fv(Weiss); glVertex3f(x + width, y - height, z + width);	// Front bottom point
			
	glEnd();

	// Now render the bottom of our pyramid

	glBegin(GL_QUADS);

		// These vertices create the bottom of the pyramid
		glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z + width);	// Front left point
		glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z + width);    // Front right point
		glColor3ub(0, 0, 255); glVertex3f(x + width, y - height, z - width);    // Back right point
		glColor3ub(0, 0, 255); glVertex3f(x - width, y - height, z - width);    // Back left point
	glEnd();
}