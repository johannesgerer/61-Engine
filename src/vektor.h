#ifndef _vektor_H
#define _vektor_H

struct CVector3
{
public:
	
	// A default constructor
	CVector3() {x=0;y=0;z=0;};

	// This is our constructor that allows us to initialize our data upon creating an instance
	CVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	void RenderVektor(CVector3 start,float size);
	// Here we overload the + operator so we can add vectors together 
	CVector3 operator+(CVector3 vVector)
	{
		// Return the added vectors result.
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	CVector3 operator-(CVector3 vVector)
	{
		// Return the subtracted vectors result
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	CVector3 operator*(float num)
	{
		// Return the scaled vector
		return CVector3(x * num, y * num, z * num);
	}

	float operator*(CVector3 vec)
	{
		// Return the scaled vector
		return x*vec.x+y*vec.y+z*vec.z;
	}
	// Here we overload the / operator so we can divide by a scalar
	CVector3 operator/(float num)
	{
		// Return the scale vector
		return CVector3(x / num, y / num, z / num);
	}

	float x, y, z;						
};

class CZiel{

public:
	CZiel(){};
	CVector3 m_p;


};


CVector3 Cross(CVector3 vVector1, CVector3 vVector2);
float Magnitude(CVector3 vNormal);
float Magnitude(float x);
CVector3 Normalize(CVector3 vVector);
void VectorPrint(CVector3* vector,const char* string);
void VectorPrint(CVector3 vector,const char* string);
CVector3 RotateVector (float angle,CVector3 axis, CVector3 vView);
void RenderVektor(CVector3 start,float size);
#endif
