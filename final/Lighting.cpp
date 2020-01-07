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
//
//  Contributor:  Zizhun Guo on 2019/12/12
///

#include "Lighting.h"
#include "Shapes.h"
#include "Utils.h"
#include <iostream>

#ifdef __cplusplus
using namespace std;
#endif

// Add any global variables you need here.

// These variables are used in Phong shading
GLfloat cap_grey_specExp  = 50.0f;
GLfloat cap_green_specExp = 50.0f;
GLfloat ground_specExp  = 1000.0f;

GLfloat k[3]         = { 0.5f, 0.7f, 1.0f };


// lighting data that needs to be available elsewhere
GLfloat lpDefault[4]  = { 15.0f, 18.0f, 20.0f, 1.0f };
GLfloat lightpos[4]   = { 15.0f, 18.0f, 20.0f, 1.0f };

// private lighting data
static GLfloat lightcolor[4] = {  1.0f,  1.0f,  1.0f, 1.0f };
static GLfloat amblight[4]   = {  0.7f,  0.7f,  0.7f, 1.0f };

// 
// Three sets of material properties.
// Green cone: CAP3
// Grey cone & spheres: CAP1 CAP2 CAP4
// Dark green ground: GROUND
//

GLfloat cap_grey_ambient[4]      = { 0.488f, 0.480f, 0.480f, 1.00f };
GLfloat cap_grey_diffuse[4]      = { 0.488f, 0.480f, 0.480f, 1.00f };
GLfloat cap_green_ambient[4]      = { 0.24f, 0.35f, 0.32f, 1.00f };
GLfloat cap_green_diffuse[4]      = { 0.24f, 0.35f, 0.32f, 1.00f };
GLfloat cap_blue_ambient[4]      = { 0.27f, 0.523f, 0.94f, 1.00f };
GLfloat cap_blue_diffuse[4]      = { 0.27f, 0.523f, 0.94f, 1.00f };
GLfloat ground_ambient[4]      = { 0.48f, 0.59f, 0.41f, 1.00f };
GLfloat ground_diffuse[4]      = { 0.48f, 0.59f, 0.41f, 1.00f };

GLfloat specular[4]         = { 1.00f, 1.00f, 1.00f, 1.00f };
///
// This function sets up the lighting parameters for the shaders.
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setLighting( GLuint program, int obj)
{
    switch( obj ) {
        case CAP1:
        case CAP2:
        case CAP4:
            glUniform1f( getUniformLoc( program, "specExp" ), cap_grey_specExp );
            glUniform4fv( getUniformLoc( program, "specularColor" ), 1, specular );
            break;
        case CAP3: // sets nothing because cap3 (cone) using "flat shader"
            break;
        case GROUND:
            glUniform1f( getUniformLoc( program, "specExp" ), ground_specExp );
            glUniform4fv( getUniformLoc( program, "specularColor" ), 1, specular );
            break;
        case SILO1:
        case SILO2:
        case BARN1:
        case BARN2:
            glUniform1f( getUniformLoc( program, "specExp" ), ground_specExp );
            break;
    }
    glUniform3fv( getUniformLoc( program, "kCoeff" ), 1, k );

    switch( obj ) {
        case CAP1:
        case CAP2:
            glUniform4fv( getUniformLoc( program, "ambientColor" ), 1, cap_grey_ambient );
            glUniform4fv( getUniformLoc( program, "diffuseColor" ), 1, cap_grey_diffuse );
            break;
        case CAP4:
            glUniform4fv( getUniformLoc( program, "ambientColor" ), 1, cap_blue_ambient );
            glUniform4fv( getUniformLoc( program, "diffuseColor" ), 1, cap_blue_diffuse );
            break;
        case CAP3:
            glUniform4fv( getUniformLoc( program, "ambientColor" ), 1, cap_green_ambient );
            glUniform4fv( getUniformLoc( program, "diffuseColor" ), 1, cap_green_diffuse );
            break;
        case GROUND:
            glUniform4fv( getUniformLoc( program, "ambientColor" ), 1, ground_ambient );
            glUniform4fv( getUniformLoc( program, "diffuseColor" ), 1, ground_diffuse );
            break;
	}

    // Lighting parameters
    glUniform4fv( getUniformLoc( program, "lightPosition" ), 1, lightpos );
    glUniform4fv( getUniformLoc( program, "lightColor" ), 1, lightcolor );
    glUniform4fv( getUniformLoc( program, "ambientLight" ), 1, amblight );
}
