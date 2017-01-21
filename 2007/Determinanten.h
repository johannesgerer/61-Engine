//################################
//Determinanten.h (letzte Änderung: 01.05.04 19:54)
//################################
/*
Diese Datei enthält verschiedene Funktionen, die Determinanten berechnen können

1) double det(CVektor v1,CVektor v2,CVektor v3):
	//Determinatne 3-er Vektoren ohne Beachtung der homogenen Koordinate

2) double det(CMat A, int r, int s):
	//Determinante einer 4x4 Matrix, ohne ihrer r-ten Reihe und s-ten Spalte

3)double det(CMat A):
	//Determinante einer 4x4 Matrix
*/

#pragma once

//Determinatne 3-er Vektoren ohne Beachtung der homogenen Koordinate
double det(CVektor v1,CVektor v2,CVektor v3);

//Determinante einer 4x4 Matrix, ohne ihrer r-ten Reihe und s-ten Spalte
double det(CMat A, int r, int s);

//Determinante einer 4x4 Matrix
double det(CMat A);
