///
//  Lab.h
//
//  Assignment-specific code for this assignment.
//
//  Created by Warren R. Carithers on 2019/09/09.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
///

#ifndef _LAB_H_
#define _LAB_H_

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "Canvas.h"

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
extern int w_width;
extern int w_height;

// drawing window title
extern const char *w_title;

// GL context we're using (we assume 3.0, for GLSL 1.30)
extern int gl_maj;
extern int gl_min;

// our GLFWwindow
extern GLFWwindow *w_window;

// our Canvas
extern Canvas *C;

// variables related to drawing the clipping regions
extern BufferSet clipBuffers;

// variables related to drawing the original polygons
extern BufferSet polyOutlineBuffers;

// variables related to drawing the resulting polygons
extern BufferSet clippedPolyBuffers;

// shader program handle
extern GLuint program;

// names of our GLSL shader files (we assume GLSL 1.30)
extern const char *vshader;
extern const char *fshader;

///
// PUBLIC FUNCTIONS
///

///
// OpenGL initialization
///
bool labInit( int argc, char *argv[] );

///
// Event loop for this assignment
///
void labLoop( void );

#endif
