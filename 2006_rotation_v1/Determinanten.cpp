#include "Mathe.h"

//Determinante einer 4x4 Matrix, ohne ihrer r-ten Reihe und s-ten Spalte
double det(CMat A, int r, int s)
{
	CVektor	v[3];
	int	r_abstand=0;

	for(int	i=0;i<4;i++)
	{
		int	s_abstand=0;

		if(i==r)
			r_abstand=1;
		else
			for(int	j=0;j<4;j++)
				if(j==s)
					s_abstand=1;
				else
					v[i-r_abstand].Koordinate(j-s_abstand,A.r[i](j));
	}

	return det(v[0],v[1],v[2]);	
}

//Determinatne 3-er Vektoren ohne Beachtung der homogenen Koordinate
double det(CVektor v1,CVektor v2,CVektor v3)			
{
	return (v1^v2)*v3;
}


//Determinatne einer 4x4 Matrix
double det(CMat A)
{
	double wert=0;
	for(int i=0;i<4;i++)
		wert+=A.r[i].w*det(A,i,3);					
		//4.Koordinate der i. Spalte mal Determinante von A ohne Reihe i und ohne 3. Spalte
	return wert;
	//http://www.itl.nist.gov/div898/software/dataplot/refman2/ch4/mat_dete.pdf
}
