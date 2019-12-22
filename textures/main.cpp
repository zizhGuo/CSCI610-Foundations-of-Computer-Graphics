///
//  main.cpp
//
//  Main program for assignments.
//
//  Created by Warren R. Carithers on 02/27/14.
//  Updates: 2019/09/09 by wrc.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
///

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

#include "ShaderSetup.h"
#include "Vertex.h"
#include "Color.h"

#include "Application.h"

using namespace std;

///
// Event callback routines
//
// We define a general GLFW callback routine; all others must be
// defined and registered in the assignment-specific code.
///

///
// Error callback for GLFW
///
void glfwError( int code, const char *desc )
{
    cerr << "GLFW error " << code << ": " << desc << endl;
    exit( 2 );
}


///
// Main program for this assignment
///
int main( int argc, char *argv[] )
{
    glfwSetErrorCallback( glfwError );

    if( !glfwInit() ) {
        cerr << "Can't initialize GLFW!" << endl;
        exit( 1 );
    }

    // w_width, w_height, and w_title come from the Application module
    w_window = glfwCreateWindow( w_width, w_height, w_title, NULL, NULL );

    if( !w_window ) {
        cerr << "GLFW window create failed!" << endl;
	glfwTerminate();
	exit( 1 );
    }

    glfwMakeContextCurrent( w_window );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        glfwTerminate();
        exit( 1 );
    }

    if( !GLEW_VERSION_3_0 ) {
        cerr << "OpenGL 3.0 not available" << endl;
	if( !GLEW_VERSION_2_1 ) {
	    cerr << "OpenGL 2.1 not available, either!" << endl;
            glfwTerminate();
            exit( 1 );
	}
    }
#endif

    // determine whether or not we can use GLSL 1.30
    gl_maj = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MAJOR );
    gl_min = glfwGetWindowAttrib( w_window, GLFW_CONTEXT_VERSION_MINOR );

    cerr << "GLFW: using " << gl_maj << "." << gl_min << " context" << endl;

    // set up OpenGL and register any required callbacks
    if( !appInit(argc,argv) ) {
        // something went wrong somewhere
	glfwTerminate();
	exit( 1 );
    }

    // loop until it's time to quit
    appLoop();

    // all done - shut everything down cleanly
    glfwDestroyWindow( w_window );
    glfwTerminate();

    return 0;
}
