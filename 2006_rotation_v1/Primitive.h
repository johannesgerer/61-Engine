class CMat;
class CVektor;

void ZeichneWuerfel(float a, int farbe, bool normale=0);

void ZeichneQuadrat(float a, float abstand, int xyz, int farbe, int Drehsinn=1, bool normale=0);
void ZeichneVierreck(CVektor p1, CVektor p2, CVektor p3, CVektor p4, int farbe, int Drehsinn=1, bool normale=0);

//Rechteck um den Ursprung der gewählten Ebene
void ZeichneRechteck(float a_halbe, float b_halbe, float abstand, int xyz, int farbe, int Drehsinn=1, bool normale=0);

void ZeichneDreieck(CVektor p1, CVektor p2, CVektor p3, int farbe1, int farbe2, int farbe3, int Drehsinn=1, bool normale=0);
void ZeichneDreieck(CVektor p1, CVektor p2, CVektor p3, int farbe, int Drehsinn=1, bool normale=0);

void ZeichneKreis(int farbe,int seiten, double radius);