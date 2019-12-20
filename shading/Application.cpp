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
#include "Utils.h"

#include "Application.h"
#include "Shapes.h"
#include "Viewing.h"
#include "Lighting.h"

using namespace std;

///
// PRIVATE GLOBALS
///

///
// Object information
///

// animation flag
static bool animating = false;

// current shader type
static int currentShader;

// do we need to do a display() call?
static bool updateDisplay = true;

// object animation rotation angles
static GLfloat angles[2] = { 0.0f, 0.0f };

// torus transformations
static GLfloat tor_scale[] = {  2.5f, 2.5f,  2.5f };
static GLfloat tor_xlate[] = { -1.5f, 0.5f, -1.5f };

// teapot transformations
static GLfloat tea_scale[] = { 2.0f, 2.0f,  2.0f };
static GLfloat tea_xlate[] = { 1.0f, 0.5f, -1.5f };

// names of our GLSL shader files
static const char *flat_vs    = "flat.vert";
static const char *flat_fs    = "flat.frag";
static const char *phong_vs   = "phong.vert";
static const char *phong_fs   = "phong.frag";

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

// drawing window title
const char *w_title = (char *) "Shading";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Canvas
Canvas *canvas;

// We need four vertex buffers and four element buffers:
// one each for flat shading and one each for non-flat shading.
//
// Array layout:
//         column 0      column 1
// row 0:  torus flat    torus non-flat
// row 1:  teapot flat   teapot non-flat
//
BufferSet buffers[2][2];

// shader program handles
GLuint program, flat, phong;

// do we want to report errors?
bool quietMode = false;

///
// PRIVATE FUNCTIONS
///

///
// createShape() - create vertex and element buffers for a shape
//
// @param object   which shape to create
// @param shading  flat vs. non-flat shading
///
static void createShape( int object, int shading )
{
    // clear any previous shape
    canvas->clear();

    // make the shape
    makeShape( object, shading, *canvas );

    // create the buffers
    buffers[object][shading].createBuffers( *canvas );
}

///
// Animate the objects (maybe)
///
static void animate( void ) {
    if( animating ) {
        angles[TORUS]  += 2.0f;
        angles[TEAPOT] += 1.0f;
        updateDisplay = true;
    }
}

///
// Event callback routines for this assignment
///

///
// Handle keyboard input
///
static void keyboard( GLFWwindow *window, int key, int scan,
                      int action, int mods )
{
    switch( key ) {
    case GLFW_KEY_1:   // flat shading
        program = flat;
        currentShader = FLAT;
        break;

    case GLFW_KEY_2:   // Phong shading
        program = phong;
        currentShader = NOT_FLAT;
        break;

    case GLFW_KEY_A:   // start animating
        animating = true;
        break;

    case GLFW_KEY_S:   // stop animating
        animating = false;
        break;

    case GLFW_KEY_R:   // reset rotation angles
        angles[0] = 0.0f;
        angles[1] = 0.0f;
        break;

    case GLFW_KEY_ESCAPE:  // terminate the program
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;
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

    // set up projection parameters
    setFrustum( program );

    // set up the camera
    setCamera( program );

    checkErrors( "display common" );

    // transformations for the torus
    GLfloat a1[] = { angles[TORUS], angles[TORUS], angles[TORUS] };
    setTransforms( program, tor_scale, a1, tor_xlate );

    // illumination for the torus
    setLighting( program, TORUS, currentShader );

    // draw it
    buffers[TORUS][currentShader].selectBuffers( program,
        "vPosition", NULL, "vNormal", NULL );

    checkErrors( "display torus" );

    glDrawElements( GL_TRIANGLES, buffers[TORUS][currentShader].numElements,
        GL_UNSIGNED_INT, (void *)0 );

    // program = phong;
    // currentShader = NOT_FLAT;
    // glUseProgram( program );
    
    // transformations for the teapot
    GLfloat a2[] = { angles[TEAPOT], angles[TEAPOT], angles[TEAPOT] };
    setTransforms( program, tea_scale, a2, tea_xlate );

    // illumination for the tenumapot
    setLighting( program, TEAPOT, currentShader );

    // draw it
    buffers[TEAPOT][currentShader].selectBuffers( program,
        "vPosition", NULL, "vNormal", NULL );

    checkErrors( "display teapot" );

    glDrawElements( GL_TRIANGLES, buffers[TEAPOT][currentShader].numElements,
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
    int num = 1;  // starting using 'flat' shading

    // see if we had a requested starting place
    if( argc > 1 ) {
        int i = (int) strtol( argv[1], NULL, 10 );
        if( i < 1 || i > 2 ) {
	    cerr << "Bad shader number '" << i << "' ignored" << endl;
        } else {
            num = i;
        }
    }

    // Create our Canvas "object"
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
	return( false );
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    flat = shaderSetup( flat_vs, flat_fs, &error );
    if( !flat ) {
        cerr << "Error setting up flat shaders - "
	    << errorString(error) << endl;
	return( false );
    }

    phong = shaderSetup( phong_vs, phong_fs, &error );
    if( !phong ) {
        cerr << "Error setting up phong shaders - "
	    << errorString(error) << endl;
	return( false );
    }

    // select the starting shader program
    switch( num ) {
    case 1:
        program = flat;
        currentShader = FLAT;
        break;
    case 2:
        program = phong;
        currentShader = NOT_FLAT;
        break;
    default:
        cerr << "error - bad shader number " << num << endl;
        return( false );
    }

    glUseProgram( program );

    checkErrors( "init 1" );

    // Create all four objects
    createShape( TORUS, FLAT );
    createShape( TORUS, NOT_FLAT );
    createShape( TEAPOT, FLAT );
    createShape( TEAPOT, NOT_FLAT );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    checkErrors( "init 2" );

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );

    checkErrors( "init 3" );

    return( true );
}

///
// Event loop for this assignment
///
void appLoop( void )
{
    checkErrors( "pre-loop" );
    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( w_window );
            checkErrors( "loop" );
        }
        glfwPollEvents();  // non-blocking check for events
    }
}
