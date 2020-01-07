///
//  Textures.h
//
//  Simple class for setting up texture mapping parameters.
//
//  Created by Warren R. Carithers 2016/11/22.
//  Updates: 2019/11/23 by wrc.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This code can be compiled as either C or C++.
//  
//  This file should not be modified by students.
///

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#include <stdbool.h>
#endif

// Are we doing texture mapping instead of material properties?
extern bool usingTextures;

///
// This function initializes all texture-related data structures for
// the program.  This is where texture buffers should be created, where
// texture images should be loaded, etc.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
///
void initTextures( void );

///
// This function sets up the parameters for texture use.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param obj     The object type of the object being drawn
///
void setTextures( GLuint program, int obj );

#endif 
