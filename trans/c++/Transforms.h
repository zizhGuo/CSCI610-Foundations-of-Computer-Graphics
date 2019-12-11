///
//  Transforms.h
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
//  Contributor:  YOUR_NAME_HERE
///

#ifndef _TRANSFORMS_H_
#define _TRANSFORMS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
// Viewing modes
///

typedef
    enum vmode_e {
        Frustum, Ortho
    } ViewMode;

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
void setProjection( GLuint program, ViewMode mode );

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
void clearTransforms( GLuint program );

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
                    GLfloat rotate[], GLfloat xlate[] );

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
void clearCamera( GLuint program );

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
void setCamera( GLuint program, GLfloat eye[], GLfloat lookat[], GLfloat up[] );

#endif
