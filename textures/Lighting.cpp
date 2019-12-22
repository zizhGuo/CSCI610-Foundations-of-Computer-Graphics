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

// Add any global variables you need here.

// lighting data that needs to be available elsewhere
GLfloat lpDefault[4]  = { 15.0f, 18.0f, 20.0f, 1.0f };
GLfloat lightpos[4]   = { 15.0f, 18.0f, 20.0f, 1.0f };

// private lighting data
static GLfloat lightcolor[4] = {  1.0f,  1.0f,  1.0f, 1.0f };
static GLfloat amblight[4]   = {  0.7f,  0.7f,  0.7f, 1.0f };

///
// This function sets up the lighting parameters for the shaders.
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setLighting( GLuint program )
{
    GLint loc;

    // Lighting parameters
    loc = getUniformLoc( program, "lightPosition" );
    glUniform4fv( loc, 1, lightpos );

    loc = getUniformLoc( program, "lightColor" );
    glUniform4fv( loc, 1, lightcolor );

    loc = getUniformLoc( program, "ambientLight" );
    glUniform4fv( loc, 1, amblight );
}
