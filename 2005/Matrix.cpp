#include "Mathe.h"

void CMat::Schreiben(bool rechts, int Zeile, bool w)//Schreibt die 4 Zeilen der Matrix rechts oder links auf den Bildschirm
{
	for(int i=0;i<4;i++)
		r[i].Schreiben(rechts,Zeile+i,w);
}

//########## START: Konstuktoren: ##########
	CMat::CMat()//Einheitsmatrix
	{
		r[0]=CVektor(1,0,0);
		r[1]=CVektor(0,1,0);
		r[2]=CVektor(0,0,1);
		r[3]=CVektor();
	}

	CMat::CMat	/*9 Werte für die ersten 3 Spalten und Reihen*/(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33)
	{
		r[0]=CVektor(a11,a12,a13);
		r[1]=CVektor(a21,a22,a23);
		r[2]=CVektor(a31,a32,a33);
		r[3]=CVektor();
	}
	CMat::CMat	/*alle 16 Werte der 4x4 Matrix*/(double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44)
	{
		r[0]=CVektor(a11,a12,a13,a14);
		r[1]=CVektor(a21,a22,a23,a24);
		r[2]=CVektor(a31,a32,a33,a34);
		r[3]=CVektor(a41,a42,a43,a44);
	}
	CMat::CMat(CVektor v1, CVektor v2, CVektor v3, CVektor v4, bool spalten)	//Konstruktor mit Spalten/Zeilenvektoren
	{
		r[0]=v1; r[1]=v2; r[2]=v3; r[3]=v4;

		if(spalten)
			*this= ~(*this); //Rückgabe der Transportierten
	};
//########## STOPP: Konstuktoren: ##########


//########## START: Operatoren: ##########
CVektor	CMat::operator [](int i)		//Gibt einen Zeilenvektor zurück
{
	return r[i];
}
CVektor	CMat::operator ()(int i)		//Gibt einen Spaltenvektor zurück
{
	switch(i)
	{
	case 0:
		return CVektor(r[0].x,r[1].x,r[2].x,r[3].x);
	case 1:
		return CVektor(r[0].y,r[1].y,r[2].y,r[3].y);
	case 2:
		return CVektor(r[0].z,r[1].z,r[2].z,r[3].z);
	case 3:
		return CVektor(r[0].w,r[1].w,r[2].w,r[3].w);
	}
	return CVektor();
}

//Setzt einen Spaltenvektor
void CMat::Spalte(int i,CVektor s)	//Setzt einen Spaltenvektor
{
	switch(i)
	{
	case 0: 
		r[0].x=s.x; r[1].x=s.y; r[2].x=s.z; r[3].x=s.w;
		break;
	case 1:
		r[0].y=s.x; r[1].y=s.y; r[2].y=s.z; r[3].x=s.w;
		break;
	case 2:
		r[0].z=s.x; r[1].z=s.y; r[2].z=s.z; r[3].x=s.w;
		break;
	case 3:
		r[0].w=s.x; r[1].w=s.y; r[2].z=s.z; r[3].x=s.w;
		break;
	}
	return;
}

	CMat CMat::operator ~()				//Transportierte einer Matrix
	{
		return CMat(this->operator ()(0),this->operator ()(1),this->operator ()(2),this->operator ()(3),0);
	}

	CMat CMat::operator *(CMat A)		//Matrixmultiplikation
	{
		CMat AB=CMat();
		for(int i=0;i<4;i++)
			AB.r[i]=CVektor(r[i]*A(0),r[i]*A(1),r[i]*A(2),r[i]*A(3));
		return AB;
	}

	CMat CMat::operator +(CMat A)		//Matrixaddition
	{	
		CMat AB=CMat();
		for(int i=0;i<3;i++)
			AB.r[i]=r[i]+A.r[i];
		return AB;
	}

	CMat CMat::operator -(CMat A)		//Subtraktion
	{	
		CMat AB=CMat();
		for(int i=0;i<3;i++)
			AB.r[i]=r[i]-A.r[i];
		return AB;
	}
	CMat CMat::operator *(double a)	//Matrix mal Skalar
	{
		return CMat(r[0]*a,r[1]*a,r[2]*a);
	}
	CMat CMat::operator /(double a)	//Matrix durch Skalar
	{
		return *this*(1/a);
	}
	CMat CMat::operator !()				//Inverse einer Matrix
	{
		CMat A;
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				A.r[i].Koordinate(j,hoch(-1,i+j)*det(*this,j,i));
		return A/det(*this);
	}
	void CMat::operator +=(CMat M)		//Addition
	{
		*this=*this+M;
		return;
	}
	void CMat::operator -=(CMat M)		//Subtraktion
	{
		*this=*this-M;
		return;
	}
	void CMat::operator *=(CMat M)		//Matrixmultiplikation
	{
		*this=M**this;
		return;
	}
	CVektor CMat::operator *(CVektor v)				//Matrix mal Vektor
	{
	return CVektor(r[0]*v,r[1]*v,r[2]*v,r[3]*v);
	} 


//########## STOPP: Operatoren: ##########