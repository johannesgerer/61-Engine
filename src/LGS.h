//CVektor LGS4loesen(	float a11, float a12, float a13, float a14, 
//					float a21, float a22, float a23, float a24,
//					float a31, float a32, float a33, float a34)
//{
//	float d=	a11*a22*a33+a12*a23*a31+a13*a21*a32-a13*a22*a31-a11*a23*a32-a12*a21*a33;
//	float d1=	a14*a22*a33+a12*a23*a34+a13*a24*a32-a13*a22*a34-a14*a23*a32-a12*a24*a33;
//	float d2=	a11*a24*a33+a14*a23*a31+a13*a21*a34-a13*a24*a31-a11*a23*a34-a14*a21*a33;
//	float d3=	a11*a22*a34+a12*a24*a31+a14*a21*a32-a14*a22*a31-a11*a24*a32-a12*a21*a34;
//	return CVektor(d1/d,	d2/d,	d3/d);
//}

CVektor LGS4loesen(CVektor v1,CVektor v2,CVektor v3,CVektor v4)
{
	double d =det(v1,v2,v3);
	return CVektor(det(v4,v2,v3)/d,	det(v1,v4,v3)/d,	det(v1,v2,v4)/d);
}
