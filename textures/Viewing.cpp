///
//  Viewing
//
//  Simple module for setting up the projection and camera transformations
//  for the texture mapping assignment.
//
//  Created by Warren R. Carithers 2019/11/23.
//  Based on the similar module in the shading assignment.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

// need memcpy()
#ifdef __cplusplus
#include <cstring>
#else
#include <string.h>
#endif

#include "Viewing.h"
#include "Vector.h"
#include "Utils.h"

#ifdef __cplusplus
using namespace std;
#endif

///
// Pseudo-function to convert degrees to radians
///

#define	MY_PI	3.141592654f

#define	D2R(d)	((d) * MY_PI / 180.0f)


// standard camera information
static GLfloat eye[] =    { 0.2f, 3.0f, 6.5f };
static GLfloat lookat[] = { 0.0f, 1.0f, 0.0f };
static GLfloat up[] =     { 0.0f, 1.0f, 0.0f };

// clipping boundaries: left, right, top, bottom, near, far
static GLfloat bounds[] = { -1.0f, 1.0f, 1.0f, -1.0f, 3.0f, 100.5f };

#define LEFT	bounds[0]
#define RIGHT	bounds[1]
#define TOP	bounds[2]
#define BOTTOM	bounds[3]
#define NEAR	bounds[4]
#define FAR	bounds[5]

///
// This function sets up a frustum projection of the scene.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setFrustum( GLuint program )
{
    GLfloat pmat[16] = {
	// column 0
        (2.0f * NEAR) / (RIGHT - LEFT), 0.0f, 0.0f, 0.0f,
	// column 1
	0.0f, (2.0f * NEAR) / (TOP - BOTTOM), 0.0f, 0.0f,
	// column 2
	(RIGHT + LEFT) / (RIGHT - LEFT), (TOP + BOTTOM) / (TOP - BOTTOM),
	  (-1.0f * (FAR + NEAR)) / (FAR - NEAR), -1.0f,
	// column 3
	0.0f, 0.0f, (-2.0f * FAR * NEAR) / (FAR - NEAR), 0.0f
    };

    GLint loc = getUniformLoc( program, "pMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, pmat );
}

///
// This function sets up the transformation parameters for the vertices
// of the object.  The order of application is fixed: scaling, Z rotation,
// Y rotation, X rotation, and then translation.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param scale  - scale factors for each axis
// @param rotate - rotation angles around the three axes, in degrees
// @param xlate  - amount of translation along each axis
///
void setTransforms( GLuint program, GLfloat scale[],
                    GLfloat rotate[], GLfloat xlate[] )
{
#if 0
printf( "xforms: scale %4.2f %4.2f %4.2f rotate %4.2f %4.2f %4.2f xlate %4.2f %4.2f %4.2f\n",
scale[0], scale[1], scale[2],
rotate[0], rotate[1], rotate[2],
xlate[0], xlate[1], xlate[2] );
#endif
    // create translation and scale matrices
    GLfloat tmat[16] = {
        // column 0
	1.0f, 0.0f, 0.0f, 0.0f,
        // column 1
	0.0f, 1.0f, 0.0f, 0.0f,
        // column 2
	0.0f, 0.0f, 1.0f, 0.0f,
        // column 3
	xlate[0], xlate[1], xlate[2], 1.0f
    };

    GLint loc = getUniformLoc( program, "tMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, tmat );

    GLfloat smat[16] = {
        scale[0], 0.0f, 0.0f, 0.0f,
	0.0f, scale[1], 0.0f, 0.0f,
	0.0f, 0.0f, scale[2], 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
    };

    loc = getUniformLoc( program, "sMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, smat );

    // create the three rotation matrices
    float rads[3]    = { D2R(rotate[0]), D2R(rotate[1]), D2R(rotate[2]) };
    float cosines[3] = { cos(rads[0]), cos(rads[1]), cos(rads[2]) };
    float sines[3]   = { sin(rads[0]), sin(rads[1]), sin(rads[2]) };

    //float radx =D2R(rotate[0]), rady = D2R(rotate[1]), radz = D2R(rotate[2]);
    //float sinx = sin(radx), siny = sin(rady), sinz = sin(radz);
    //float cosx = cos(radx), cosy = cos(rady), cosz = cos(radz);

    GLfloat xmat[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, cosines[0], sines[0], 0.0f,
	0.0f, -sines[0], cosines[0], 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
    };

    loc = getUniformLoc( program, "xMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, xmat );

    GLfloat ymat[16] = {
        cosines[1], 0.0f, -sines[1], 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	sines[1], 0.0f, cosines[1], 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
    };

    loc = getUniformLoc( program, "yMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, ymat );

    GLfloat zmat[16] = {
        cosines[2], sines[2], 0.0f, 0.0f,
	-sines[2], cosines[2], 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
    };

    loc = getUniformLoc( program, "zMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, zmat );
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setCamera( GLuint program )
{
    ///
    // Begin by calculating the axes of the camera coordinate system
    ///

    // calculate N
    Vector tmp = { eye[0] - lookat[0], eye[1] - lookat[1], eye[2] - lookat[2] };

    // normalize it
    Vector N;
    norm( N, tmp );

    // calculate U
    Vector U, uvec;

    // convert 'up' to a Vector
    uvec[0] = up[0];
    uvec[1] = up[1];
    uvec[2] = up[2];

    norm( tmp, uvec );      // normalize 'up'
    cross( uvec, tmp, N );  // cross 'up' with 'N'
    norm( U, uvec );       // normalize the result

    // calculate V
    Vector V;

    cross( tmp, N, U );
    norm( V, tmp );

    ///
    // Next, create the camera matrix
    ///

    // convert eye to vector form
    Vector evec;
    memcpy( evec, eye, 3 * sizeof(GLfloat) );

    // compute the dot products
    float dotue = -1.0f * dot( U, evec );
    float dotve = -1.0f * dot( V, evec );
    float dotne = -1.0f * dot( N, evec );

    // create the matrix in column-major order
    GLfloat vmat[16] = {
	// column 0
        U[0], V[0], N[0], 0.0f,
	// column 1
        U[1], V[1], N[1], 0.0f,
	// column 2
        U[2], V[2], N[2], 0.0f,
	// column 3
	dotue, dotve, dotne, 1.0f
    };

    // copy it down to the shader program
    GLint loc = getUniformLoc( program, "vMat" );
    glUniformMatrix4fv( loc, 1, GL_FALSE, vmat );
}
