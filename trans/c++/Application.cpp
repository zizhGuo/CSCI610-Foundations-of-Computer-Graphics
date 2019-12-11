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

#include "Application.h"
#include "Transforms.h"
#include "Shapes.h"

using namespace std;

///
// PRIVATE GLOBALS
///

///
// Transformation information
///

// alternate camera parameters
static GLfloat alt_eye[] =    { 0.0f, 1.3f, -0.5f };
static GLfloat alt_lookat[] = { 0.0f, -0.4f, -1.0f };
static GLfloat alt_up[] =     { 0.0f, 1.0f, 0.0f };

// alternate transformation values
static GLfloat alt_scale[] =  { 1.0f, 2.0f, 1.0f };
static GLfloat alt_rotate[] = { 0.0f, 10.0f, 335.0f };
static GLfloat alt_xlate[] =  { -0.2f, 0.2f, 0.0f };

// do we need to do a display() call?
static bool updateDisplay = true;

// mouse click tracker
static int counter = 0;

// flags controlling drawing options
static bool cameraAdjust = false;
static bool transformsOn = false;
static int viewMode = 1;

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
//int w_width  = 512;
//int w_height = 512;
int w_width  = 1600;
int w_height = 1024;

// drawing window title
const char *w_title = (char *) "Transformations";

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
const char *vshader = "shader.vert";
const char *fshader = "shader.frag";

///
// PRIVATE FUNCTIONS
///

///
// Create the current teapot image
///
static void createShape( void )
{
    // reset the canvas
    canvas->clear();

    // make the teapot
    makeShapes( *canvas );

    // create the necessary buffers
    shapeBuffers.createBuffers( *canvas );
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
    case GLFW_KEY_F:
        viewMode = 1;
        break;

    case GLFW_KEY_O:
        viewMode = 2;
        break;

    case GLFW_KEY_1:
        counter = 0; cameraAdjust = false; transformsOn = false;
        break;

    case GLFW_KEY_2:
        counter = 1; cameraAdjust = false; transformsOn = true;
        break;

    case GLFW_KEY_3:
        counter = 2; cameraAdjust = true; transformsOn = false;
        break;

    case GLFW_KEY_4:
        counter = 3; cameraAdjust = true; transformsOn = true;
        break;

    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( w_window, 1 );
        break;
    // for final testing
    case GLFW_KEY_W:
        alt_eye[2] += 0.5f;
        cout << "alt_eye z coordinate = " << alt_eye[2] << endl;
        break;
    case GLFW_KEY_S:
        alt_eye[2] -= 0.5f;
        cout << "alt_eye z coordinate = " << alt_eye[2] << endl;
        break;
    case GLFW_KEY_E:
        alt_lookat[0] += 1.0f;
        cout << "alt_eye z coordinate = " << alt_lookat[0] << endl;
        break;
    case GLFW_KEY_C:
        alt_lookat[0] -= 1.0f;
        cout << "alt_eye z coordinate = " << alt_lookat[0] << endl;
        break;
    }

    updateDisplay = true;
}

///
// Handle mouse clicks
///
static void mouse( GLFWwindow *window, int button, int action, int mods )
{
    if( action != GLFW_PRESS )  // only activate on press, not release
        return;

    if( button == GLFW_MOUSE_BUTTON_LEFT )
        counter++;

    switch( counter % 4 ) {
    case 0:
        // default camera, no transforms
        cameraAdjust = false;
        transformsOn = false;
        break;

    case 1:
        // default camera, turn on transformations
        cameraAdjust = false;
        transformsOn = true;
        break;

    case 2:
        // modified camera, no transforms
        cameraAdjust = true;
        transformsOn = false;
        break;

    case 3:
        // modified camera, transformations
        cameraAdjust = true;
        transformsOn = true;
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

    // bind our buffers
    shapeBuffers.selectBuffers( program, "vPosition", NULL, NULL, NULL );

    // set up viewing and projection parameters
    if( viewMode == 1 ) {
        setProjection( program, Frustum );
    } else if( viewMode == 2 ) {
        setProjection( program, Ortho );
    } else {
        cerr << "unknown viewing mode " << viewMode << " - resetting" << endl;
        viewMode = 1;
        setProjection( program, Frustum );
    }

    // set up the camera
    if( cameraAdjust ) {
        setCamera( program, alt_eye, alt_lookat, alt_up );
    } else {
        clearCamera( program );
    }

    // set up transformations
    //
    // transformations are applied in this order (if you are having
    // trouble recreating the solution image, check your order of
    // matrix multiplication):
    //
    //    scale Y by 2
    //    rotate around Z by 335 degrees
    //    rotate around Y by 10 degrees
    //    translate in X by -0.2
    //    translate in Y by 0.2
    if( transformsOn ) {
        setTransforms( program, alt_scale, alt_rotate, alt_xlate );
    } else {
        clearTransforms( program );
    }

    // draw the shapes
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
    // see if we were given a starting image number
    if( argc > 1 ) {
        int num = (int) strtol( argv[1], NULL, 10 );
        if( num >= 0 && num <= 3 ) {
            counter = num;
        } else {
            cerr << "invalid image number '" << argv[1] << "' ignored" << endl;
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
        cerr << "Warning - GLSL 1.30 shaders may not compile" << endl;
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - "
	    << errorString(error) << endl;
	return( false );
    }

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createShape();

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );
    glfwSetMouseButtonCallback( w_window, mouse );

    return( true );
}

///
// Event loop for this assignment
///
void appLoop( void )
{
    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        if(  updateDisplay ) {
            updateDisplay = false;
            display();
	    glfwSwapBuffers( w_window );
        }
	glfwWaitEvents();
    }
}
