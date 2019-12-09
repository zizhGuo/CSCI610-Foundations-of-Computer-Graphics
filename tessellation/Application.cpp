///
//  Application.cpp
//
//  Assignment-specific code for this assignment.
//
//  Created by Warren R. Carithers on 2019/09/09.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

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
#include "Buffers.h"
#include "Canvas.h"
#include "Shapes.h"

#include "Application.h"

#define CUBE            0
#define CYLINDER        1
#define CONE            2
#define SPHERE          3

using namespace std;

///
// PRIVATE GLOBALS
///

// the shape currently being drawn
static int currentShape = CUBE;

// subdivisions for tessellation
static int factor1 = 1;
static int factor2 = 1;

// are we animating?
static bool animating = false;

// do we need to do a display() call?
static bool updateDisplay = true;

// rotation control
static const float anglesReset[3] = { 30.0, 30.0, 0.0 };
static float angles[3] = { 30.0, 30.0, 0.0 };
static float angleInc = 5.0;

// shader variable location
static GLint theta;

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 512;
int w_height = 512;

// drawing window title
const char *w_title = (char *) "Tessellation";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Canvas
Canvas *canvas;

// buffers for our shapes
BufferSet shapeBuffers;

// shader program handle
GLuint program;

// names of our GLSL shader files (we assume GLSL 1.30)
const char *vshader = "v130.vert";
const char *fshader = "v130.frag";

///
// PRIVATE FUNCTIONS
///

///
// Create a new shape by tesselating one of the 4 basic objects
///
static void createNewShape( void ) {

    // clear the old shape
    canvas->clear();

    // create the new shape..
    switch( currentShape ) {
    case CUBE:
        makeCube( *canvas, factor1 );
        break;
    case CYLINDER:
        makeCylinder( *canvas, 0.5, factor1, factor2 );
        break;
    case CONE:
        makeCone( *canvas, 0.5, factor1, factor2 );
        break;
    case SPHERE:
        makeSphere( *canvas, 0.5, factor1, factor2 );
        break;
    default:
        cerr << "Unknown shape number '"<< currentShape
	     << "' - drawing cube" << endl;
        currentShape = CUBE;
        makeCube( *canvas, factor1 );
    }

    // create our buffers
    shapeBuffers.createBuffers( *canvas );
}

///
// Animation routine
//
// Note: Can cause gimbal lock which also happened on Apollo 11
// http://en.wikipedia.org/wiki/Gimbal_lock#Gimbal_lock_on_Apollo_11
// Solution? Use Quaternions (Taught in Comp. Animation: Algorithms)
//
// TIDBIT:
// Quaternion plaque on Brougham (Broom) Bridge, Dublin, which says:
//
//    "Here as he walked by
//    on the 16th of October 1843
//
//    Sir William Rowan Hamilton
//
//    in a flash of genius discovered
//    the fundamental formula for
//    quaternion multiplication
//    i^2 = j^2 = k^2 = ijk = -1
//    & cut it on a stone of this bridge"
///
static void animate( void ) {
    int i;
    static int level = 0;

    // only want to animate 450 iterations, then stop
    if( level >= 450 ) {
        level = 0;
        animating = false;
    }

    if( !animating ) {
        return;
    }

    if( level < 150 ) {
        angles[0] -= angleInc / 3;
    } else if( level < 300 ) {
        angles[1] -= angleInc / 3;
    } else {
        angles[2] -= angleInc / 3;
    }

    ++level;
    updateDisplay = true;
}

///
// Event callback routines for this assignment
///

///
// Handle keyboard input
//
// We need the actual characters, not just generic character names,
// so we register this using glfwSetCharCallback() instead of the
// more general glfwSetKeyCallback().
///
static void keyboard( GLFWwindow *window, unsigned int codepoint )
{
    // look only at the low-order ASCII character value
    switch( codepoint & 0x7f ) {

    // termination
    case 033:  // FALL THROUGH
    case 'q':  // FALL THROUGH
    case 'Q':
        glfwSetWindowShouldClose( w_window, 1 );
        break;

    // automated animation
    case 'a':  // FALL THROUGH
    case 'A':
        animating = true;
        break;

    case 's':  // FALL THROUGH
    case 'S':
        animating = false;
        break;

    // incremental rotation along the axes
    case 'x':
        angles[0] -= angleInc;
        break;
    case 'X':
        angles[0] += angleInc;
        break;

    case 'y':
        angles[1] -= angleInc;
        break;
    case 'Y':
        angles[1] += angleInc;
        break;

    case 'z':
        angles[2] -= angleInc;
        break;
    case 'Z':
        angles[2] += angleInc;
        break;

    // shape selection
    case '1':
        currentShape = CUBE;
        createNewShape();
        break;

    case '2':
        currentShape = CYLINDER;
        createNewShape();
        break;

    case '3':
        currentShape = CONE;
        createNewShape();
        break;

    case '4':
        currentShape = SPHERE;
        createNewShape();
        break;

    // tessellation factors
    case '+':  // factor 1 is used by all shapes
        factor1++;
        createNewShape();
        break;

    case '-':
        if( factor1 > 1 ) {
            factor1--;
            createNewShape();
        }
        break;

    case '=':  // factor 2 is used by all except the cube
        factor2++;
        if( currentShape != CUBE ) createNewShape();
        break;

    case '_':
        if( factor2 > 1)  {
            factor2--;
            // cube doesn't use this tessellation factor
            if( currentShape != CUBE ) createNewShape();
        }
        break;

    // reset rotation angles
    case 'r':
        angles[0] = anglesReset[0];
        angles[1] = anglesReset[1];
        angles[2] = anglesReset[2];
        break;

    // reset tessellation factors
    case 'R':
        factor1 = factor2 = 1;
        createNewShape();
        break;

    // print rotation angles
    case 'p':
        cerr << "Rotation angles:  "
             << angles[0] << ", " << angles[1] << ", " << angles[2] << endl;
        // return without updating the display
        return;

    // print tessellation factors
    case 'P':
	cerr << stderr << "Tessellation factors:  "
	     << factor1 << ", " << factor2 << endl;
	// return without updating the display
	return;

    default:
        cerr << "Unknown codepoint input: 0x"
	     << hex << codepoint << dec << endl;
        // return without updating the display
        return;
    }

    updateDisplay = true;
}

///
// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the shader program
    glUseProgram( program );

    // bind the vertex and element buffers, and set up
    // the attribute variables
    shapeBuffers.selectBuffers( program, "vPosition", NULL, NULL, NULL );

    // set the rotation parameter
    glUniform3fv( theta, 1, angles );

    // draw our shape
    glDrawElements( GL_TRIANGLES, shapeBuffers.numElements,
                    GL_UNSIGNED_INT, (void *)0 );
}

///
// PUBLIC FUNCTIONS
///

///
// OpenGL initialization
///
bool appInit( int argc, char *argv[] )
{
    // process any command-line request
    if( argc > 1 ) {
        int num = (int) strtol( argv[1], NULL, 10 );
        if( num >= 1 && num <= 4 ) {
            currentShape = num - 1;
        } else {
            fprintf( stderr, "invalid shape number '%s' ignored\n", argv[1] );
        }
    }

    // Create our Canvas "object"
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
	return( false );
    }

    // Check the OpenGL major version
    if( gl_maj < 3 ) {
        vshader = "v120.vert";
	fshader = "v120.frag";
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - "
	    << errorString(error) << endl;
	return( false );
    }

    // select the shader program here, and remember
    // where the rotation variable is
    glUseProgram( program );
    theta = glGetUniformLocation( program, "theta" );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createNewShape();

    // register our callbacks
    glfwSetCharCallback( w_window, keyboard );

    return( true );
}

///
// Event loop for this assignment
///
void appLoop( void )
{
    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
	animate();
	if( updateDisplay ) {
	    updateDisplay = false;
            display();
	    glfwSwapBuffers( w_window );
	}
	glfwPollEvents();  // non-blocking check for events
    }
}
