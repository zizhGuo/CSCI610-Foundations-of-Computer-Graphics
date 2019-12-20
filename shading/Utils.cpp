///
//  Utils
//
//  Various utility functions.
//
//  Created by Warren R. Carithers on 2019/11/18.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#ifdef __cplusplus
#include <cstdlib>
// wimp out and use stdio
#include <cstdio>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Utils.h"
#include "Application.h"

#ifdef __cplusplus
using namespace std;
#endif

///
// OpenGL error checking
//
// @param msg  message prefix to print with each error message
///
void checkErrors( const char *msg ) {
    GLenum code;
    const char *str;

    while( (code = glGetError()) != GL_NO_ERROR ) {
	if( !quietMode ) {
	    fprintf( stderr, "*** %s, GL error code 0x%x: ", msg, code );
            switch( code ) {
	    case GL_INVALID_ENUM:
	        str = "Invalid enum";
	        break;
	    case GL_INVALID_VALUE:
	        str = "Invalid value";
	        break;
	    case GL_INVALID_OPERATION:
	        str = "Invalid operation";
	        break;
	    case GL_STACK_OVERFLOW:
	        str = "Stack overflow";
	        break;
	    case GL_STACK_UNDERFLOW:
	        str = "Stack underflow";
	        break;
	    case GL_OUT_OF_MEMORY:
	        str = "Out of memory";
	        break;
#ifndef __APPLE__
	    // these don't exist on some Mac OpenGL installations
	    case GL_INVALID_FRAMEBUFFER_OPERATION:
	        str = "Invalid FB operation";
	        break;
	    case GL_CONTEXT_LOST:
	        str = "Context lost";
	        break;
#endif
	    default:
	        str = "*** UNKNOWN ERROR";
	    }
	    fprintf( stderr, "%s\n", str );
	}
    }
}

///
// Dump the list of active attributes and uniforms
//
// @param program  which shader program to query
///
void dumpActives( GLuint program ) {
     GLuint i;
     GLint count, size;
     GLsizei length;
     GLenum type;
     const GLsizei bufSize = 32;
     GLchar name[bufSize];

    glGetProgramiv( program, GL_ACTIVE_ATTRIBUTES, &count );
    if( count < 1 ) {
         fputs( "No ", stderr );
    }
    fputs( "Active attributes\n", stderr );
    for( i = 0; i < count; ++i ) {
        glGetActiveAttrib( program, i, bufSize, &length, &size, &type, name );
	fprintf( stderr, "  #%u Type: %u Name: '%s'\n", i, type, name );
    }

    glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &count );
    if( count < 1 ) {
         fputs( "No ", stderr );
    }
    fputs( "Active uniforms\n", stderr );
    for( i = 0; i < count; ++i ) {
        glGetActiveUniform( program, i, bufSize, &length, &size, &type, name );
	fprintf( stderr, "  #%u Type: %u Name: '%s'\n", i, type, name );
    }
}

///
// Retrieve a Uniform variable's location and verify the result
//
// @param program  the shader program
// @param name     the name of the desired variable
///
GLint getUniformLoc( GLuint program, const GLchar *name ) {
    GLint loc;

    loc = glGetUniformLocation( program, name );
    if( !quietMode && loc < 0 ) {
        fprintf( stderr, "Bad uniform, program %u variable '%s'\n",
	    program, name );
    }

    return( loc );
}

///
// Retrieve an Attribute variable's location and verify the result
//
// @param program  the shader program
// @param name     the name of the desired variable
///
GLint getAttribLoc( GLuint program, const GLchar *name ) {
    GLint loc;

    loc = glGetAttribLocation( program, name );
    if( !quietMode && loc < 0 ) {
        fprintf( stderr, "Bad attribute, program %u variable '%s'\n",
	    program, name );
    }

    return( loc );
}
