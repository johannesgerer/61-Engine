#include "main.h"
#include <math.h>

void VectorPrint(CVector3* vector,const char* string)
{
	glColor3f(1, 1, 1);
	glDrawText(0, 20+26*zeilen++, string,Magnitude(*vector),vector->x,vector->y,vector->z);
}
void VectorPrint(CVector3 vector,const char* string)
{
	glColor3f(1, 1, 1);
	glDrawText(0, 20+26*zeilen++, string,Magnitude(vector),vector.x,vector.y,vector.z);
}


CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;	

	// Calculate the cross product with the non communitive equation
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product
	return vNormal;										 
}


/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(CVector3 vNormal)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return (float)sqrt( (vNormal.x * vNormal.x) + 
						(vNormal.y * vNormal.y) + 
						(vNormal.z * vNormal.z) );
}
float Magnitude(float x)
{
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
	return Magnitude(CVector3(x,0,0));
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 Normalize(CVector3 vVector)
{
	// Get the magnitude of our normal
	float magnitude = Magnitude(vVector);				

	// Now that we have the magnitude, we can divide our vector by that magnitude.
	// That will make our vector a total length of 1.  
	vVector = vVector / magnitude;		
	
	// Finally, return our normalized vector
	return vVector;										
}


CVector3 RotateVector(float angle, CVector3 axis, CVector3 vView)
{
	CVector3 vNewView;

	float x=axis.x;
	float y=axis.y;
	float z=axis.z;

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	return vNewView;

}
void CVector3::RenderVektor(CVector3 start,float size)
{
	CVector3 ende=start+CVector3(x,y,z);
	ende=ende*size;

	glBegin(GL_LINES);
		glVertex3f(start.x,start.y,start.z);
		glVertex3f(ende.x,ende.y,ende.z);
	glEnd();
}