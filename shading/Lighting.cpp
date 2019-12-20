///
//  Lighting
//
//  Simple module for setting up the parameters for lighting and shading
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This code can be compiled as either C or C++.
///

#include "Lighting.h"
#include "Shapes.h"
#include "Utils.h"

#ifdef __cplusplus
using namespace std;
#endif

// TORUS		0
// TEAPOT		1

// FLAT		0
// NOT_FLAT	1

// Add any global variables you need here.
// teapot
static GLfloat tp_OA[] = {0.5, 0.1, 0.9, 1.0};
static GLfloat tp_KA = 0.5f;
static GLfloat tp_OD[] = {0.89, 0.0, 0.0, 1.0};
static GLfloat tp_KD = 0.7f;
static GLfloat tp_OS[] = {1.0, 1.0, 1.0, 1.0 };
static GLfloat tp_SE = 10.0f;
static GLfloat tp_KS = 1.0f;

//torus
static GLfloat tr_OA[] = {0.1, 0.85, 0.2, 1.0 };
static GLfloat tr_KA = 0.5f;
static GLfloat tr_OD[] = {0.1, 0.85, 0.2, 1.0};
static GLfloat tr_KD = 0.7f;
static GLfloat tr_OS[] = {1.0, 1.0, 1.0, 1.0 };
static GLfloat tr_SE = 50.0f;
static GLfloat tr_KS = 1.0f;

//light source
static GLfloat IS[] = {1.0, 1.0, 0.0, 1.0};
static GLfloat lpos[] = {0.0, 5.0, 2.0, 1.0}; 
static GLfloat IA[] = {0.5, 0.5, 0.5, 1.0};

///
// This function sets up the lighting, material, and shading parameters
// for the shaders.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param object   which object is currently being drawn
// @param shading  type of shading being used
///
void setLighting( GLuint program, int object, int shading )
{
    // Add your code here.
	// getting locations of uniform variables and assigning data to them
	GLint loc = getUniformLoc(program, "IS");
	glUniform4fv(loc, 1, IS);
	loc = getUniformLoc(program, "lpos");
	glUniform4fv(loc, 1, lpos);
	loc = getUniformLoc(program, "IA");
	glUniform4fv(loc, 1, IA);


    if (object == TORUS)
    {
    	loc = getUniformLoc(program, "OA");
    	glUniform4fv(loc, 1, tr_OA);
    	loc = getUniformLoc(program, "KA");
    	glUniform1f(loc, tr_KA);
    	loc = getUniformLoc(program, "OD");
    	glUniform4fv(loc, 1, tr_OD);
    	loc = getUniformLoc(program, "KD");
    	glUniform1f(loc, tr_KD);

    	if (shading == NOT_FLAT)
		{
    		loc = getUniformLoc(program, "OS");
	    	glUniform4fv(loc,1, tr_OS);
	    	loc = getUniformLoc(program, "SE");
	    	glUniform1f(loc, tr_SE);
	    	loc = getUniformLoc(program, "KS");
	    	glUniform1f(loc, tr_KS);
		}

    }
    if (object == TEAPOT)
    {
    	loc = getUniformLoc(program, "OA");
    	glUniform4fv(loc, 1, tp_OA);
    	loc = getUniformLoc(program, "KA");
    	glUniform1f(loc, tp_KA);
    	loc = getUniformLoc(program, "OD");
    	glUniform4fv(loc, 1, tp_OD);
    	loc = getUniformLoc(program, "KD");
    	glUniform1f(loc, tp_KD);
    	
    	if (shading == NOT_FLAT)
		{
	    	loc = getUniformLoc(program, "OS");
	    	glUniform4fv(loc, 1, tp_OS);
	    	loc = getUniformLoc(program, "SE");
	    	glUniform1f(loc, tp_SE);
	    	loc = getUniformLoc(program, "KS");
	    	glUniform1f(loc, tp_KS);	
		}
    }

}
