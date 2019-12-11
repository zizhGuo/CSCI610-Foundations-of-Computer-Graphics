///
//  Transforms
//
//  Simple module for setting up the model and camera transformations
//  for the Transformation Assignment.
//
//  Created by Warren R. Carithers 2016/11/11.
//  Updates: 2019/04/04, 2019/10/12 by wrc.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  Contributor:  Zizhun Guo
///

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Transforms.h"

// standard camera information
static GLfloat std_eye[] =    { 0.0f, 0.0f, 10.0f };
static GLfloat std_lookat[] = { 0.0f, 0.0f, -1.0f };
static GLfloat std_up[] =     { 0.0f, 1.0f, 0.0f };
// static GLfloat std_eye[] =    { 0.0f, 0.0f, 0.0f };
// static GLfloat std_lookat[] = { -2.5f, 0.0f, -1.0f };
// static GLfloat std_up[] =     { 0.0f, 1.0f, 0.0f };

// default transformations
static GLfloat std_scale[] =  { 7.5f, 10.0f, 7.50f };
static GLfloat std_rotate[] = { 0.0f, 0.0f, 0.0f };
static GLfloat std_xlate[] =  { 0.0f, 0.0f, 0.0f };
// static GLfloat std_scale[] =  { 1.0f, 5.0f, 32.0f };
// static GLfloat std_rotate[] = { 0.0f, 0.0f, 0.0f };
// static GLfloat std_xlate[] =  { 13.0f, 4.80f, 18.39f };

// clipping boundaries: left, right, top, bottom, near, far
//                          0      1     2     3      4     5
//static GLfloat bounds[] = { -1.0f, 1.0f, 1.0f, -1.0f, 0.9f, 4.5f };
static GLfloat bounds[] = { -1.0f, 1.0f, 1.0f, -1.0f, 0.9f, 70.0f };
///
// This function sets up the view and projection parameters for the
// desired projection of the scene. See the assignment description for
// the values for the projection parameters.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param mode - The desired projection mode (Frustum or Ortho)
///
void setProjection( GLuint program, ViewMode mode )
{
    
	GLuint projLoc = glGetUniformLocation(program, "projection");

    // Add your code here.

    // Construct projection transformation outside of shader in order to
    // utilize glUniformMatrix4fv() function.
    // Two modes: Frustum and orthographic.
    if (mode == Frustum){
		GLfloat projTrans[16] = {2 * bounds[4] / (bounds[1] - bounds[0]), 0.0f, 0.0f, 0.0f,
							0.0f, 2 * bounds[4] / (bounds[2] - bounds[3]), 0.0f, 0.0f,
							(bounds[1] + bounds[0])/(bounds[1] - bounds[0]), (bounds[2] + bounds[3])/(bounds[2] - bounds[3]), - (bounds[5] + bounds[4])/(bounds[5] - bounds[4]), -1.0f,
							0.0f, 0.0f, - 2 * bounds[5] * bounds[4]/(bounds[5] - bounds[4]), 0.0f};
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, projTrans);  	
    }
    if (mode == Ortho){
		GLfloat projTrans[16] = {2 / (bounds[1] - bounds[0]), 0.0f, 0.0f, 0.0f,
							0.0f, 2 / (bounds[2] - bounds[3]), 0.0f, 0.0f,
							0.0f, 0.0f, - 2 / (bounds[5] - bounds[4]), 0.0f,
							- (bounds[1] + bounds[0])/(bounds[1] - bounds[0]), - (bounds[2] + bounds[3])/(bounds[2] - bounds[3]), - (bounds[5] + bounds[4])/(bounds[5] - bounds[4]), 1.0f};    		
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, projTrans);    	
    }
}

///
// This function clears any model transformations, setting the values
// to the defaults.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearTransforms( GLuint program )
{
    // Add your code here.
    GLuint scaleLoc = glGetUniformLocation(program, "scale"); // Get attribute location
    glUniform3fv(scaleLoc, 1, std_scale); // set value to that attribute

    GLuint rotateLoc = glGetUniformLocation(program, "rotate");
    glUniform3fv(rotateLoc, 1, std_rotate);

    GLuint translateLoc = glGetUniformLocation(program, "translate");
    glUniform3fv(translateLoc, 1, std_xlate);
}

///
// This function sets up the transformation parameters for the vertices
// of the teapot.  The order of application is specified in the driver
// program.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
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
    // Add your code here.
    GLuint scaleLoc = glGetUniformLocation(program, "scale");
    glUniform3fv(scaleLoc, 1, scale);

    GLuint rotateLoc = glGetUniformLocation(program, "rotate");
    glUniform3fv(rotateLoc, 1, rotate);

    GLuint translateLoc = glGetUniformLocation(program, "translate");
    glUniform3fv(translateLoc, 1, xlate);
}

///
// This function clears any changes made to camera parameters, setting the
// values to the defaults.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void clearCamera( GLuint program )
{
    // Add your code here.
    GLuint eyepointLoc = glGetUniformLocation(program, "eyepoint");
    glUniform3fv(eyepointLoc, 1, std_eye);

    GLuint lookatLoc = glGetUniformLocation(program, "lookat");
    glUniform3fv(lookatLoc, 1, std_lookat);

    GLuint upLoc = glGetUniformLocation(program, "up");
    glUniform3fv(upLoc, 1, std_up);
}

///
// This function sets up the camera parameters controlling the viewing
// transformation.
//
// You will need to write this function, and maintain all of the values
// which must be sent to the vertex shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param eye    - camera location
// @param lookat - lookat point
// @param up     - the up vector
///
void setCamera( GLuint program, GLfloat eye[], GLfloat lookat[], GLfloat up[] )
{
    // Add your code here.
    GLuint eyepointLoc = glGetUniformLocation(program, "eyepoint");
    glUniform3fv(eyepointLoc, 1, eye);

    GLuint lookatLoc = glGetUniformLocation(program, "lookat");
    glUniform3fv(lookatLoc, 1, lookat);

    GLuint upLoc = glGetUniformLocation(program, "up");
    glUniform3fv(upLoc, 1, up);
}
