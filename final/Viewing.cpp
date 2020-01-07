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
//  Contributor:  Zizhun Guo 2019/12/12
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

#include <iostream>
#include "Viewing.h"
#include "Vector.h"
#include "Utils.h"
#include "Shapes.h"

#ifdef __cplusplus
using namespace std;
#endif

///
// Pseudo-function to convert degrees to radians
///

#define	MY_PI	3.141592654f

#define	D2R(d)	((d) * MY_PI / 180.0f)


// standard camera information
static GLfloat eye[] =    { -1.1f, -0.699f, 12.5f };
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


// sets camera eye vector x coordinate up
void eye_up_x(GLfloat offset) {
    eye[0] += offset;
    cout << "Camera lookat: X = " << eye[0] << endl; 
}

// sets camera eye vector x coordinate down
void eye_down_x(GLfloat offset) {eye[0] -= offset;
    cout << "Camera lookat: X = " << eye[0] << endl;
}

// sets camera eye vector y coordinate up
void eye_up_y(GLfloat offset) {eye[1] += offset;
    cout << "Camera lookat: Y = " << eye[1] << endl;
}

// sets camera eye vector y coordinate down
void eye_down_y(GLfloat offset) {eye[1] -= offset;
    cout << "Camera lookat: Y = " << eye[1] << endl;
}

// sets camera eye vector z coordinate up
void eye_up_z(GLfloat offset) {eye[2] += offset;
    cout << "Camera lookat: Z = " << eye[2] << endl;
}

// sets camera eye vector z coordinate down
void eye_down_z(GLfloat offset) {eye[2] -= offset;
    cout << "Camera lookat: Z = " << eye[2] << endl;
}

// The initial scale & translate vectors for all objects
GLfloat silo1_s[3]   = {  1.5f,  5.7f,  1.1f };
GLfloat silo1_x[3]   = { -2.35f, 1.0f, -1.5f };
GLfloat cap1_s[3]    = {  1.5f,  2.3f,  0.7f };
GLfloat cap1_x[3]    = {  -2.4f, 3.9f, -1.9f };
GLfloat silo2_s[3] = {  0.9f,  4.7f,  0.7f };
GLfloat silo2_x[3] = {  -1.3f,  0.6f, 0.4f };
GLfloat cap2_s[3]   = {  1.0f,  1.4f,  1.6f };
GLfloat cap2_x[3]   = { -1.35f, 3.4f, -1.4f };
GLfloat barn1_s[3]    = {  5.1f,  2.2f,  5.0f };
GLfloat barn1_x[3]    = {  2.0f, -0.7f, -2.2f };
GLfloat cap3_s[3] = {  5.0f,  3.0f,  5.2f };
GLfloat cap3_x[3] = {  1.8f,  1.8f, -1.2f };
GLfloat barn2_s[3]    = {  0.6f,  0.6f,  0.6f };
GLfloat barn2_x[3]    = {  1.8f, 3.2f, -1.2f };
GLfloat cap4_s[3] = {  0.7f,  0.5f,  0.5f };
GLfloat cap4_x[3] = {  1.8f,  3.7f, -1.1f };
GLfloat ground_s[3] = {  11.2f,  1.7f,  2.9f };
GLfloat ground_x[3] = {  0.1f,  -2.7f, -2.9f };

// the current object
int object = SILO1;

// do we need to do a display() call?
bool updateDisplay = true;

void keyboard( GLFWwindow *window, unsigned int codepoint);

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

///
// Event callback routines for this assignment
///

///
// Handle keyboard input
// We need the actual characters, not just generic character names,
// so we register this using glfwSetCharCallback() instead of the
// more general glfwSetKeyCallback().
///
void keyboard( GLFWwindow *window, unsigned int codepoint)
{
    // look only at the low-order ASCII character value
    codepoint &= 0x7f;

    switch( codepoint ) {

    case '1':  // set the current object to SILO1
        object = SILO1;
        cout << "current object = " << object << endl;
        break;
    case '2':  // set the current object to CAP1
        object = CAP1;
        cout << "current object = " << object << endl;
        break;
    case '3':  // set the current object to SILO2
        object = SILO2;
        cout << "current object = " << object << endl;
        break;
    case '4':  // set the current object to CAP2
        object = CAP2;
        cout << "current object = " << object << endl;
        break;
    case '5':  // set the current object to BARN1
        object = BARN1;
        cout << "current object = " << object << endl;
        break;
    case '6':  // set the current object to CAP3
        object = CAP3;
        cout << "current object = " << object << endl;
        break;
    case '7':  // set the current object to BARN2
        object = BARN2;
        cout << "current object = " << object << endl;
        break;
    case '8':  // set the current object to CAP4
        object = CAP4;
        cout << "current object = " << object << endl;
        break;
    case '9':  // set the current object to GROUND
        object = GROUND;
        cout << "current object = " << object << endl;
        break;
    
    // set scale coordinates

    // press 'q' to increase current object's scale x-coordinates by 0.1f
    case 'q':
        if (object == SILO1){
            cout << "silo1_s X = " << silo1_s[0] << endl;
            silo1_s[0] += 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_s X = " << cap1_s[0] << endl;
            cap1_s[0] += 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_s X = " << silo2_s[0] << endl;
            silo2_s[0] += 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_s X = " << cap2_s[0] << endl;
            cap2_s[0] += 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_s X = " << barn1_s[0] << endl;
            barn1_s[0] += 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_s X = " << cap3_s[0] << endl;
            cap3_s[0] += 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_s X = " << barn2_s[0] << endl;
            barn2_s[0] += 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_s X = " << cap4_s[0] << endl;
            cap4_s[0] += 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_s X = " << ground_s[0] << endl;
            ground_s[0] += 0.1f; break;
        }

    // press 'a' to decrease current object's scale x-coordinates by 0.1f
    case 'a':
        if (object == SILO1){
            cout << "silo1_s X = " << silo1_s[0] << endl;
            silo1_s[0] -= 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_s X = " << cap1_s[0] << endl;
            cap1_s[0] -= 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_s X = " << silo2_s[0] << endl;
            silo2_s[0] -= 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_s X = " << cap2_s[0] << endl;
            cap2_s[0] -= 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_s X = " << barn1_s[0] << endl;
            barn1_s[0] -= 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_s X = " << cap3_s[0] << endl;
            cap3_s[0] -= 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_s X = " << barn2_s[0] << endl;
            barn2_s[0] -= 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_s X = " << cap4_s[0] << endl;
            cap4_s[0] -= 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_s X = " << ground_s[0] << endl;
            ground_s[0] -= 0.1f; break;
        }

    // press 'w' to increase current object's scale y-coordinates by 0.1f
    case 'w':
        if (object == SILO1){
            cout << "silo1_s Y = " << silo1_s[1] << endl;
            silo1_s[1] += 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_s Y = " << cap1_s[1] << endl;
            cap1_s[1] += 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_s Y = " << silo2_s[1] << endl;
            silo2_s[1] += 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_s Y = " << cap2_s[1] << endl;
            cap2_s[1] += 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_s Y = " << barn1_s[1] << endl;
            barn1_s[1] += 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_s Y = " << cap3_s[1] << endl;
            cap3_s[1] += 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_s Y = " << barn2_s[1] << endl;
            barn2_s[1] += 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_s Y = " << cap4_s[1] << endl;
            cap4_s[1] += 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_s Y = " << ground_s[1] << endl;
            ground_s[1] += 0.1f; break;
        }

    // press 's' to decrease current object's scale y-coordinates by 0.1f
    case 's':
        if (object == SILO1){
            cout << "silo1_s Y = " << silo1_s[1] << endl;
            silo1_s[1] -= 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_s Y = " << cap1_s[1] << endl;
            cap1_s[1] -= 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_s Y = " << silo2_s[1] << endl;
            silo2_s[1] -= 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_s Y = " << cap2_s[1] << endl;
            cap2_s[1] -= 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_s Y = " << barn1_s[1] << endl;
            barn1_s[1] -= 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_s Y = " << cap3_s[1] << endl;
            cap3_s[1] -= 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_s Y = " << barn2_s[1] << endl;
            barn2_s[1] -= 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_s Y = " << cap4_s[1] << endl;
            cap4_s[1] -= 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_s Y = " << ground_s[1] << endl;
            ground_s[1] -= 0.1f; break;
        }

    // press 'e' to increase current object's scale z-coordinates by 0.1f
    case 'e':
        if (object == SILO1){
            cout << "silo1_s Z = " << silo1_s[2] << endl;
            silo1_s[2] += 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_s Z = " << cap1_s[2] << endl;
            cap1_s[2] += 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_s Z = " << silo2_s[2] << endl;
            silo2_s[2] += 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_s Z = " << cap2_s[2] << endl;
            cap2_s[2] += 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_s Z = " << barn1_s[2] << endl;
            barn1_s[2] += 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_s Z = " << cap3_s[2] << endl;
            cap3_s[2] += 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_s Z = " << barn2_s[2] << endl;
            barn2_s[2] += 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_s Z = " << cap4_s[2] << endl;
            cap4_s[2] += 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_s Z = " << ground_s[2] << endl;
            ground_s[2] += 0.1f; break;
        }

    // press 'd' to decrease current object's scale z-coordinates by 0.1f
    case 'd':
        if (object == SILO1){
            cout << "silo1_s Z = " << silo1_s[2] << endl;
            silo1_s[2] -= 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_s Z = " << cap1_s[2] << endl;
            cap1_s[2] -= 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_s Z = " << silo2_s[2] << endl;
            silo2_s[2] -= 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_s Z = " << cap2_s[2] << endl;
            cap2_s[2] -= 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_s Z = " << barn1_s[2] << endl;
            barn1_s[2] -= 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_s Z = " << cap3_s[2] << endl;
            cap3_s[2] -= 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_s Z = " << barn2_s[2] << endl;
            barn2_s[2] -= 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_s Z = " << cap4_s[2] << endl;
            cap4_s[2] -= 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_s Z = " << ground_s[2] << endl;
            ground_s[2] -= 0.1f; break;
        }

    // X modification
    // press 'r' to increase current object's translate x-coordinates by 0.1f
    case 'r':
        if (object == SILO1){
            cout << "silo1_x X = " << silo1_x[0] << endl;
            silo1_x[0] += 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_x X = " << cap1_x[0] << endl;
            cap1_x[0] += 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_x X = " << silo2_x[0] << endl;
            silo2_x[0] += 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_x X = " << cap2_x[0] << endl;
            cap2_x[0] += 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_x X = " << barn1_x[0] << endl;
            barn1_x[0] += 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_x X = " << cap3_x[0] << endl;
            cap3_x[0] += 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_x X = " << barn2_x[0] << endl;
            barn2_x[0] += 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_x X = " << cap4_x[0] << endl;
            cap4_x[0] += 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_x X = " << ground_x[0] << endl;
            ground_x[0] += 0.1f; break;
        }

    // press 'f' to decrease current object's translate x-coordinates by 0.1f
    case 'f':
        if (object == SILO1){
            cout << "silo1_x X = " << silo1_x[0] << endl;
            silo1_x[0] -= 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_x X = " << cap1_x[0] << endl;
            cap1_x[0] -= 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_x X = " << silo2_x[0] << endl;
            silo2_x[0] -= 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_x X = " << cap2_x[0] << endl;
            cap2_x[0] -= 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_x X = " << barn1_x[0] << endl;
            barn1_x[0] -= 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_x X = " << cap3_x[0] << endl;
            cap3_x[0] -= 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_x X = " << barn2_x[0] << endl;
            barn2_x[0] -= 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_x X = " << cap4_x[0] << endl;
            cap4_x[0] -= 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_x X = " << ground_x[0] << endl;
            ground_x[0] -= 0.1f; break;
        }

    // press 't' to increase current object's translate y-coordinates by 0.1f
    case 't':
        if (object == SILO1){
            cout << "silo1_x Y = " << silo1_x[1] << endl;
            silo1_x[1] += 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_x Y = " << cap1_x[1] << endl;
            cap1_x[1] += 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_x Y = " << silo2_x[1] << endl;
            silo2_x[1] += 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_x Y = " << cap2_x[1] << endl;
            cap2_x[1] += 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_x Y = " << barn1_x[1] << endl;
            barn1_x[1] += 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_x Y = " << cap3_x[1] << endl;
            cap3_x[1] += 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_x Y = " << barn2_x[1] << endl;
            barn2_x[1] += 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_x Y = " << cap4_x[1] << endl;
            cap4_x[1] += 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_x Y = " << ground_x[1] << endl;
            ground_x[1] += 0.1f; break;
        }

    // press 'g' to decrease current object's translate y-coordinates by 0.1f
    case 'g':
        if (object == SILO1){
            cout << "silo1_x Y = " << silo1_x[1] << endl;
            silo1_x[1] -= 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_x Y = " << cap1_x[1] << endl;
            cap1_x[1] -= 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_x Y = " << silo2_x[1] << endl;
            silo2_x[1] -= 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_x Y = " << cap2_x[1] << endl;
            cap2_x[1] -= 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_x Y = " << barn1_x[1] << endl;
            barn1_x[1] -= 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_x Y = " << cap3_x[1] << endl;
            cap3_x[1] -= 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_x Y = " << barn2_x[1] << endl;
            barn2_x[1] -= 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_x Y = " << cap4_x[1] << endl;
            cap4_x[1] -= 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_x Y = " << ground_x[1] << endl;
            ground_x[1] -= 0.1f; break;
        }

    // press 'y' to increase current object's translate z-coordinates by 0.1f
    case 'y':
        if (object == SILO1){
            cout << "silo1_x Z = " << silo1_x[2] << endl;
            silo1_x[2] += 0.1f; break;
        }
        if (object == CAP1){
            cout << "cap1_x Z = " << cap1_x[2] << endl;
            cap1_x[2] += 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_x Z = " << silo2_x[2] << endl;
            silo2_x[2] += 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_x Z = " << cap2_x[2] << endl;
            cap2_x[2] += 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_x Z = " << barn1_x[2] << endl;
            barn1_x[2] += 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_x Z = " << cap3_x[2] << endl;
            cap3_x[2] += 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_x Z = " << barn2_x[2] << endl;
            barn2_x[2] += 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_x Z = " << cap4_x[2] << endl;
            cap4_x[2] += 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_x Z = " << ground_x[2] << endl;
            ground_x[2] += 0.1f; break;
        }

    // press 'h' to decrease current object's translate z-coordinates by 0.1f
    case 'h':
        if (object == SILO1){
            cout << "silo1_x Z = " << silo1_x[2] << endl;
            silo1_x[2] -= 0.1f; break;
        }if (object == CAP1){
            cout << "cap1_x Z = " << cap1_x[2] << endl;
            cap1_x[2] -= 0.1f; break;
        }
        if (object == SILO2){
            cout << "silo2_x Z = " << silo2_x[2] << endl;
            silo2_x[2] -= 0.1f; break;
        }
        if (object == CAP2){
            cout << "cap2_x Z = " << cap2_x[2] << endl;
            cap2_x[2] -= 0.1f; break;
        }
        if (object == BARN1){
            cout << "barn1_x Z = " << barn1_x[2] << endl;
            barn1_x[2] -= 0.1f; break;
        }
        if (object == CAP3){
            cout << "cap3_x Z = " << cap3_x[2] << endl;
            cap3_x[2] -= 0.1f; break;
        }
        if (object == BARN2){
            cout << "barn2_x Z = " << barn2_x[2] << endl;
            barn2_x[2] -= 0.1f; break;
        }
        if (object == CAP4){
            cout << "cap4_x Z = " << cap4_x[2] << endl;
            cap4_x[2] -= 0.1f; break;
        }
        if (object == GROUND){
            cout << "ground_x Z = " << ground_x[2] << endl;
            ground_x[2] -= 0.1f; break;
        }


    // Camera setting
    // press 'Z' to increase 'eye vector' x-coordinates by 0.1f
    case 'z':
        eye_up_x(0.1f);
        break;
    // press 'X' to decrease 'eye vector' x-coordinates by 0.1f
    case 'x':
        eye_down_x(0.1f);
        break;
    // press 'C' to increase 'eye vector' y-coordinates by 0.1f
    case 'c':
        eye_up_y(0.1f);
        break;
    // press 'V' to decrease 'eye vector' y-coordinates by 0.1f
    case 'v':
        eye_down_y(0.1f);
        break;
    // press 'B' to increase 'eye vector' z-coordinates by 0.1f
    case 'b':
        eye_up_z(0.1f);
        break;
    // press 'N' to decrease 'eye vector' z-coordinates by 0.1f
    case 'n':
        eye_down_z(0.1f);
        break;

    }

    updateDisplay = true;
}
