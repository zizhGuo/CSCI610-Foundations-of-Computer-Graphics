///
//  Viewing.h
//
//  Simple module for setting up the projection and camera transformations
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  .
///

#ifndef _VIEWING_H_
#define _VIEWING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>


// object transformations
// extern GLfloat quad_s[3];
// extern GLfloat quad_x[3];
// extern GLfloat cyl_s[3];
// extern GLfloat cyl_x[3];
// extern GLfloat sphere_s[3];
// extern GLfloat sphere_x[3];


extern GLfloat silo1_s[3];
extern GLfloat silo1_x[3];
extern GLfloat cap1_s[3];
extern GLfloat cap1_x[3];
extern GLfloat silo2_s[3];
extern GLfloat silo2_x[3];
extern GLfloat cap2_s[3];
extern GLfloat cap2_x[3];

extern GLfloat barn1_s[3];
extern GLfloat barn1_x[3];
extern GLfloat cap3_s[3];
extern GLfloat cap3_x[3];
extern GLfloat barn2_s[3];
extern GLfloat barn2_x[3];
extern GLfloat cap4_s[3];
extern GLfloat cap4_x[3];

extern GLfloat ground_s[3];
extern GLfloat ground_x[3];

extern int object;

// do we need to do a display() call?
extern bool updateDisplay;

extern void keyboard( GLFWwindow *window, unsigned int codepoint);

extern void eye_up_x(GLfloat offset);
extern void eye_down_x(GLfloat offset);
extern void eye_up_y(GLfloat offset);
extern void eye_down_y(GLfloat offset);
extern void eye_up_z(GLfloat offset);
extern void eye_down_z(GLfloat offset);



///
// This function sets up a frustum projection of the scene.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setFrustum( GLuint program );

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
                    GLfloat rotate[], GLfloat xlate[] );

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setCamera( GLuint program );

#endif
