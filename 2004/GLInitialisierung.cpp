#include "GLInclude.h"

void GLInitialisierung ()					// Any GL Init Code & User Initialiazation Goes Here
{
	glClearColor (0.0f, 0.0f, 0.0f, 1);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	//glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);										//Johannes Gerer
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			//Johannes Gerer
	glEnable (GL_LIGHTING);										//Johannes Gerer
	//glEnable(GL_COLOR_MATERIAL);								//Johannes Gerer
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
}