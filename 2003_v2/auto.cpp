#include "main.h"
#include "auto.h"
#include <math.h>

CF40::CF40(){

	tire[0]=CTire(0.875f,-1.0f,1.2f);
	tire[1]=CTire(0.875f,-1.0f,-1.2f);
	tire[2]=CTire(-2.625f,-1.0f,-1.2f);
	tire[3]=CTire(-2.625f,-1.0f,1.2f);

	m_radstand=(-1*tire[2].x)+tire[0].x;

	m_p=CVector3(0,1.5f,0);
	m_v=CVector3(0,0,0);
	m_a=CVector3(0,0,0);
	m_richtung=CVector3(1,0,0);

	m_v_null=CVector3(0,0,0);
	m_p_null=CVector3(0,0,0);

	m_masse=		500;
	m_gas=			7400;
	m_bremse=		10000;
	m_radeinschlag=	35.0f;
	lenkspeed=		1.5;
	luftwiderstand=	0.15125f;				//C*~0.36*A*Luftdichte
	reibung=		68.725f;	//C*30*luftwiderstand

	gas=false;
	bremse=false;

	t_null=(float)timeGetTime();

	Camera=&g_Camera;

}
CVector3 CAuto::GetAcceleration(CVector3 v){

	return (m_f-v*reibung-v*Magnitude(v)*luftwiderstand)/(m_masse);
}

float CAuto::GetMaxVelocity(){

	return (reibung-(float)sqrt((reibung*reibung)+(4*luftwiderstand*m_gas)))/((-2)*luftwiderstand);

}

bool CAuto::Movement()
{

	if(GetKeyState('W') & 0x80) {

		if(!gas){
		
//		MessageBox(NULL, "Start Beschleunigung", "Info", MB_OK);
		
		//t_null=(float)timeGetTime();
		gas=true;
		m_f=m_f+m_richtung*m_gas;
		return true;

		}
		
	}else{
		
		if(gas){

//		MessageBox(NULL, "Stopp Beschleunigung", "Info", MB_OK);

		gas=false;
		m_f=m_f-m_richtung*m_gas;
		return true;
		}
	}

	if(GetKeyState('S') & 0x80) {	

		
		if(!bremse){
			if(speed>0.5f){
				bremse=true;
				m_f=m_f-m_richtung*m_bremse;		
				return true;
			}
		}else{	
			if(speed<0.5f){
				bremse=false;
				m_f=m_f+m_richtung*m_bremse;
			}
		}
	}else{
		
		if(bremse){

		bremse=false;
		m_f=m_f+m_richtung*m_bremse;
		return true;

		}
	}


	if(GetKeyState('A') & 0x80) {


	m_winkel_lenkung-=lenkspeed*g_FrameInterval*m_radeinschlag;
	
	if(m_winkel_lenkung<-m_radeinschlag)
		m_winkel_lenkung=-m_radeinschlag;

	m_richtung=RotateVector(m_winkel_lenkung,CVector3(0,1,0),m_richtung);

	return true;

	}

	if(GetKeyState('D') & 0x80) {

		m_winkel_lenkung+=lenkspeed*g_FrameInterval*m_radeinschlag;

		if(m_winkel_lenkung>m_radeinschlag)
			m_winkel_lenkung=m_radeinschlag;
	
		return true;
	}
	if(!(GetKeyState('D') & 0x80) && !(GetKeyState('A') & 0x80))
		m_winkel_lenkung=0;

	
	return false;
}

void CAuto::update(){

	static arsch;

	if(Movement())
	{
		t_null=(float)timeGetTime();
		t=0;
	}

	t=timeGetTime()-t_null;

	speed=Magnitude(m_v);
	if(speed>96.6f/3.6f && (!arsch))
	{
		i=t;
		arsch=true;
	}

	if(Magnitude(m_f)==0 && speed<3.2f/3.6f && speed!=0)
		m_v=CVector3(0,0,0);

	m_a=GetAcceleration(m_v);
	m_v=m_v+m_a*g_FrameInterval;
	m_p=m_p+m_v*g_FrameInterval;


	float temp_mom=tire[0].y/m_radstand*m_a.x;

	tire[0].m_gewicht=m_masse*(((1-(tire[0].x/m_radstand))*9.81f)+temp_mom);
	tire[1].m_gewicht=tire[0].m_gewicht;

	tire[2].m_gewicht=m_masse*((tire[0].x/m_radstand*9.81f)-temp_mom);
	tire[1].m_gewicht=tire[0].m_gewicht;

	m_lagerung=(float)atan((-tire[0].m_gewicht/37500+tire[2].m_gewicht*3/37500)/m_radstand)*180/PI;
}

void CAuto::gas_geben(){
}

void CAuto::textout(){

			VectorPrint(&m_f,		"f/N      %.1f (%.1f, %.1f, %.1f)");
			VectorPrint(&m_a,		"a/ms-2   %.1f (%.1f, %.1f, %.1f)");
			VectorPrint(m_v*3.6f,	"v/kmh-1  %.1f (%.1f, %.1f, %.1f)");
			VectorPrint(&m_p,		"p/m      %.1f (%.1f, %.1f, %.1f)");
			VectorPrint(&m_richtung,"richtung (%.1f, %.1f, %.1f)");

//	glColor3f(1, 1, 1);
//	glDrawText(0, 20+26*(zeilen++), "lenkung: %.1f",m_winkel_lenkung);
//	glColor3f(1, 1, 1);
//	glDrawText(0, 20+26*(zeilen++), "tan: %.1f",tanl(m_winkel_lenkung*PI/180.0f));
//	glColor3f(1, 1, 1);
//	glDrawText(0, 20+26*(zeilen++), "winkel=%f",acos((m_v*m_richtung)/Magnitude(m_v)*Magnitude(m_richtung))*180.0f/PI);
//	glColor3f(1, 1, 1);
	glDrawText(0, 20+26*(zeilen++),	"time/s   %.1f",t/1000);
	glDrawText(0, 20+26*(zeilen++),	"0-100    %.1f",i/1000);
	glDrawText(0, 20+26*(zeilen++), "v_max/kmh-1   %f",GetMaxVelocity()*3.6f);
	int f=zeilen;
	glDrawText(500, 20+26*(-f+zeilen++),	"tire[0]    %.1f%%",tire[0].m_gewicht/m_masse/9.81f*100);
	glDrawText(500, 20+26*(-f+zeilen++),	"tire[2]    %.1f%%",tire[2].m_gewicht/m_masse/9.81f*100);
	glDrawText(500, 20+26*(-f+zeilen++),	"f_f/D[m]     %f%",tire[0].m_gewicht/40000);
	glDrawText(500, 20+26*(-f+zeilen++),	"f_r/D[m]     %f%",tire[2].m_gewicht*3/40000);
	glDrawText(500, 20+26*(-f+zeilen++),	"df/D[m]     %f%",-tire[0].m_gewicht/37500+tire[2].m_gewicht*3/37500);
	glDrawText(500, 20+26*(-f+zeilen++),	"alpha    %f%",m_lagerung);

	
		//glDrawText(200, 20+26*(zeilen-5), "time=%.1f",t/1000);
}


void CAuto::render(){

	float laenge=3.5f;

	int seite=1;

	glTranslatef(m_p.x,m_p.y,m_p.z);
	glRotatef(m_lagerung, 0, 0, 1);
//	m_richtung.RenderVektor(m_p,2.0f);

	float x=laenge/4;
	float y=0;
	float z=0;

	CVector3 vec=CVector3(0,5,0);
	vec.RenderVektor(CVector3(),1.0f);
	for(int i=0;i<4;i++)
	tire[i].RenderVektor(CVector3(),1);

	glBegin(GL_TRIANGLES);		
		
		glColor3ub(0, 255, 0);   
		glVertex3f(x, y-seite,z-seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x, y-seite,z+seite);
		glColor3ub(100, 110,50);
		glVertex3f(x + laenge, y, z);

		glColor3ub(0, 255, 0);   
		glVertex3f(x, y-seite,z+seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x, y+seite,z+seite);
		glColor3ub(20, 255,255);
		glVertex3f(x + laenge, y, z);

		glColor3ub(0, 255, 0);   
		glVertex3f(x, y+seite,z+seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x, y+seite,z-seite);
		glColor3ub(83, 0, 4);
		glVertex3f(x + laenge, y, z);

		glColor3ub(0, 255, 0);   
		glVertex3f(x, y-seite,z-seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x, y+seite,z-seite);
		glColor3ub(42, 100, 0);
		glVertex3f(x + laenge, y, z);

	
	glEnd();

glBegin(GL_QUADS);

		// These vertices create the bottom of the pyramid
		glColor3ub(6, 1, 100);
		glVertex3f(x-laenge, y-seite,z-seite);
		glColor3ub(168, 5, 155);
		glVertex3f(x-laenge, y-seite,z+seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x-laenge, y+seite,z+seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x-laenge, y+seite,z-seite);

		glColor3ub(16, 161, 255);
		glVertex3f(x, y-seite,z-seite);
		glColor3ub(250, 200, 255);
		glVertex3f(x, y-seite,z+seite);
		glColor3ub(255, 42, 177);
		glVertex3f(x-laenge, y-seite,z+seite);
		glColor3ub(3, 255, 255);
		glVertex3f(x-laenge, y-seite,z-seite);

		glColor3ub(0, 255, 255);
		glVertex3f(x, y-seite,z-seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x, y+seite,z-seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x-laenge, y+seite,z-seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x-laenge, y-seite,z-seite);

		glColor3ub(255, 4, 100);
		glVertex3f(x, y+seite,z-seite);
		glColor3ub(4, 255,25);
		glVertex3f(x, y+seite,z+seite);
		glColor3ub(60, 60, 70);
		glVertex3f(x-laenge, y+seite,z+seite);
		glColor3ub(3, 38, 23);
		glVertex3f(x-laenge, y+seite,z-seite);

		glColor3ub(7, 0, 255);
		glVertex3f(x, y-seite,z+seite);
		glColor3ub(0, 255, 255);
		glVertex3f(x, y+seite,z+seite);
		glColor3ub(10, 255, 100);
		glVertex3f(x-laenge, y+seite,z+seite);
		glColor3ub(89, 29, 2);
		glVertex3f(x-laenge, y-seite,z+seite);
	glEnd();

}