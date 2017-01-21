//################################
//Transformationen.h (letzte Änderung: 04.08.06 00:03)
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

//Anti-symmetrischer Tensor dritter Stufe
CMat MatKreuz(CVektor v);

//Projektion
CMat MatProjektion(float Oeffnungswinkel);
