///
//  Utils.h
//
//  Various utility functions.
//
//  Created by Warren R. Carithers on 2019/11/18.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  
///

#ifndef _UTILS_H_
#define _UTILS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
// OpenGL error checking
//
// @param msg  message prefix to print with each error message
///
void checkErrors( const char *msg );

///
// Dump the list of active attributes and uniforms from a shader program
//
// @param program  which shader program to query
///
void dumpActives( GLuint program );

///
// Retrieve a Uniform variable's location and verify the result
//
// @param program  the shader program
// @param name     the name of the desired variable
///
GLint getUniformLoc( GLuint program, const GLchar *name );

///
// Retrieve an Attribute variable's location and verify the result
//
// @param program  the shader program
// @param name     the name of the desired variable
///
GLint getAttribLoc( GLuint program, const GLchar *name );

#endif
