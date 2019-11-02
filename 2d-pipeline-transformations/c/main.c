///
//  main.c
//
//  Main program for lab assignments.
//
//  Created by Warren R. Carithers on 02/27/14.
//  Updates: 2019/09/09 by wrc.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
///

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

#include "ShaderSetup.h"
#include "Vertex.h"
#include "Color.h"

#include "Lab.h"

///
// Event callback routines
//
// We define a general GLFW callback routine; all others must be
// defined and registered in the lab-specific code.
///

///
// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
    fprintf( stderr, "GLFW error %d: %s\n", code, desc );
    exit( 2 );
}


///
// Main program for this assignment
///
int main( int argc, char *argv[] )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        fputs( "Can't initialize GLFW!\n", stderr );
        exit( 1 );
    }

    // w_width, w_height, and w_title come from the Lab module
    w_window = glfwCreateWindow( w_width, w_height, w_title, NULL, NULL );

    if( !w_window ) {
        fputs( "GLFW window create failed!\n", stderr );
	glfwTerminate();
	exit( 1 );
    }

    glfwMakeContextCurrent( w_window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_0 ) {
        fputs( "GLEW: OpenGL 3.0 not available\n", stderr );
	if( !GLEW_VERSION_2_1 ) {
	    fputs( "GLEW: OpenGL 2.1 not available, either!\n", stderr );
            glfwTerminate();
            exit( 1 );
	}
    }
#endif

    // determine whether or not we can use GLSL 1.30
    gl_maj = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MAJOR );
    gl_min = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MINOR );

    fprintf( stderr, "GLFW: using %d.%d context\n", gl_maj, gl_min );

    // set up OpenGL and register any required callbacks
    if( !labInit(argc,argv) ) {
	// something went wrong somewhere
        glfwTerminate();
	exit( 1 );
    }

    // loop until it's time to quit
    labLoop();

    // all done - shut everything down cleanly
    glfwDestroyWindow( w_window );
    glfwTerminate();

    return 0;
}
