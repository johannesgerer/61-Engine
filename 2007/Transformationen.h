//################################
//Transformationen.h (letzte Änderung: 01.05.04 19:54)
//################################
/*
Diese Datei enthält 
*/

#pragma once
class CMat;
class CVektor;

//Translation
CMat MatTrans(CVektor v);
CMat MatTrans(double x, double y, double z);

//Skalierung
CMat MatSkal(float sx, float sy, float sz);
CMat MatSkal(float s);
CMat MatSkal(CVektor s);

//Rotation
CMat MatRot_X(double a);
CMat MatRot_Z(double a);
CMat MatRot_Y(double a);
CMat MatRot(CVektor start, CVektor achse, double a);
CMat MatRot(CVektor achse, double a);

CMat MatProjektion(float Oeffnungswinkel);
