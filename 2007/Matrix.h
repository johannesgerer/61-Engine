//################################
//Matrix.h (letzte Änderung: 03.04.04 19:54)
//################################
/*
Diese Datei enthält

a) Klasse CMat. 
---------------------------------------
-eine 4x4 Matrix

-void Schreiben(bool rechts, int Zeile):
	//Schreibt die 4 Zeilen der Matrix rechts oder links auf den Bildschirm
	
-Außerdem werden folgende Operatoren definiert:
	+Addition/Subtraktion	(Matrix +/- Matrix	 = Matrix)
	+Skalarmulitplikation	(Matrix * Zahl(double)= Matrix)
	+Skalardivision			(Matrix / Zahl(double)= Matrix)
	+Matrixmultiplikation	(Matrix * Matrix	 = Matrix)
	+Inverse Matrix			(!Matrix			 = Matrix)
	+Vektortransformation	(Matrix	* Vektor	 = Vektor)

---------------------------------------------
*/


#pragma once

class CVektor;

class CMat
{
public:
//Membervariablen:
	CVektor r[4];		//4 Zeilenvektoren

//Konstruktoren:
	CMat();
		//Einheitsmatrix
	CMat(CVektor v1, CVektor v2, CVektor v3, CVektor v4=CVektor(), bool spalten=0);
		//Konstruktor mit Spalten/Zeilenvektoren
	CMat(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33);
		//9 Werte für die ersten 3 Spalten und Reihen
	CMat(double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44);
		//alle 16 Werte der 4x4 Matrix

//Member-Funktionen:
	void Spalte(int i, CVektor s);	//Setzt einen Spaltenvektor
	void Schreiben(bool rechts, int Zeile, bool w=0);

	//Operatoren:
	CVektor operator ()(int i);			//Gibt einen Spaltenvektor zurück
	CVektor operator [](int i);			//Gibt einen Zeilenvektor zurück
	void operator +=(CMat M);	//Addition
	void operator -=(CMat M);	//Subtraktion
	void operator *=(CMat M);	//Matrixmultiplikation
	
	CMat CMat::operator ~();		//Transportierte einer Matrix
	CMat CMat::operator *(CMat A);	//Matrixmultiplikation
	CMat CMat::operator +(CMat A);	//Matrixaddition
	CMat CMat::operator -(CMat A);	//Subtraktion
	CMat CMat::operator *(double a);	//Matrix mal Skalar
	CMat CMat::operator /(double a);	//Matrix durch Skalar
	CMat CMat::operator !();		//Inverse einer Matrix
	CVektor CMat::operator *(CVektor v);				//Matrix mal Vektor
};
