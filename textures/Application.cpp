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
#include "Viewing.h"
#include "Textures.h"
#include "Lighting.h"
#include "Shapes.h"
#include "Quad.h"
#include "Cylinder.h"
#include "Sphere.h"

using namespace std;

///
// PRIVATE GLOBALS
///

// object transformations
static GLfloat quad_s[3]   = {  1.5f,  1.5f,  1.5f };
static GLfloat quad_x[3]   = { -1.25f, 1.0f, -1.5f };
static GLfloat cyl_s[3]    = {  1.5f,  1.5f,  1.5f };
static GLfloat cyl_x[3]    = {  1.0f, -0.8f, -1.5f };
static GLfloat sphere_s[3] = {  1.5f,  1.5f,  1.5f };
static GLfloat sphere_x[3] = {  1.3f,  2.2f, -1.5f };

// animation control
static bool animating[N_OBJECTS];  // individual animation flags
static float angles[N_OBJECTS];    // individual rotation angles

// which object(s) to texture map
static bool map_obj[N_OBJECTS];

// do we need to do a display() call?
static bool updateDisplay = true;

// names of our GLSL shader files
static const char *vshader    = "texture.vert";
static const char *fshader    = "texture.frag";

// buffers for our shapes
static BufferSet buffers[N_OBJECTS];

// shader program handle
static GLuint program;

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
const char *w_title = "Texture mapping";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Canvas
Canvas *canvas;

///
// PRIVATE FUNCTIONS
///

///
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param C   - the Canvas to use
///
static void createShape( int obj, Canvas &C )
{
    // clear any previous shape
    canvas->clear();

    // verify the validity of the object
    if( obj < 0 || obj >= N_OBJECTS ) {
	cerr << "unknown object '" << obj << "' - drawing quad" << endl;
        obj = OBJ_QUAD;
    }

    // make the shape
    switch( obj ) {
    case OBJ_QUAD:      makeQuad( C );      break;
    case OBJ_SPHERE:    makeSphere( C );    break;
    case OBJ_CYLINDER:  makeCylinder( C );  break;
    case OBJ_DISCS:     makeDiscs( C );     break;
    default:            makeQuad( C );      break;
    }

    // create the necessary buffers
    buffers[obj].createBuffers( false, C );
}

///
// Increment or reset a rotation angle.
//
// @param obj  the object being rotated
///
static void rotate( int obj ) {
    angles[obj] += 1.0f;
    if( angles[obj] >= 360.0f ) {
        angles[obj] = 0.0f;
    }
}

///
// Animate the objects (maybe)
///
static void animate( void ) {
    if( animating[OBJ_QUAD] ) {
        rotate( OBJ_QUAD );
        updateDisplay = true;
    }
    if( animating[OBJ_SPHERE] ) {
        rotate( OBJ_SPHERE );
        updateDisplay = true;
    }
    if( animating[OBJ_CYLINDER] ) {
        rotate( OBJ_CYLINDER );
        rotate( OBJ_DISCS );
        updateDisplay = true;
    }
}

///
// Event callback routines for this assignment
///

///
// Handle keyboard input
// We need the actual characters, not just generic character names,
// so we register this using glfwSetCharCallback() instead of the
// more general glfwSetKeyCallback().
///
static void keyboard( GLFWwindow *window, unsigned int codepoint )
{
    // look only at the low-order ASCII character value
    codepoint &= 0x7f;

    switch( codepoint ) {

    // Animation control

    case 'A':  // FALL THROUGH
    case 'a':  // start/stop all animation
        animating[OBJ_QUAD] = animating[OBJ_SPHERE] =
            animating[OBJ_DISCS] = animating[OBJ_CYLINDER] =
                (codepoint == 'a');
        break;

    case 'X':  // FALL THROUGH
    case 'x':  // start/stop animating the quad
        animating[OBJ_QUAD] = codepoint == 'x';
        break;

    case 'C':  // FALL THROUGH
    case 'c':  // start/stop animating the cylinder and discs
        animating[OBJ_CYLINDER] = animating[OBJ_DISCS] = codepoint == 'c';
        break;

    case 'S':  // FALL THROUGH
    case 's':  // start/stop animating the sphere
        animating[OBJ_SPHERE] = codepoint == 's';
        break;

    // Change the light position

    case 'h':  // move the light to the left
        lightpos[0] -= 1.0f;
        break;

    case 'l':  // move the light to the right
        lightpos[0] += 1.0f;
        break;

    case 'j':  // move the light up
        lightpos[1] += 1.0f;
        break;

    case 'k':  // move the light down
        lightpos[1] -= 1.0f;
        break;

    case 'i':  // move the light into the scene
        lightpos[2] -= 1.0f;
        break;

    case 'o':  // move the light out from the scene
        lightpos[2] += 1.0f;
        break;

    // Print out potentially useful information

    case 'p':  // rotation angles
	cerr << "Rotation: quad " << angles[OBJ_QUAD]
	     << ", sphere " << angles[OBJ_SPHERE]
	     << ", cyl/disc " << angles[OBJ_CYLINDER] << endl;
        break;

    case 'P':  // light position
	cerr << "Light is at (" << lightpos[0] << "," << lightpos[1]
	     << "," << lightpos[2] << ")" << endl;
        break;

    // Reset parameters

    case 'r':  // reset all object rotations
        angles[OBJ_QUAD] = angles[OBJ_SPHERE] =
            angles[OBJ_DISCS] = angles[OBJ_CYLINDER] = 0.0f;
        break;

    case 'R':  // reset light position
        lightpos[0] = lpDefault[0];
        lightpos[1] = lpDefault[1];
        lightpos[2] = lpDefault[2];
        lightpos[3] = lpDefault[3];
        break;
    // Terminate the program
    case 033: // FALL THROUGH
    case 'q': // FALL THROUGH
    case 'Q':
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

    checkErrors( "display start" );

    // select the shader program
    glUseProgram( program );

    // set up projection parameters
    setFrustum( program );

    // set up the view transformation
    setCamera( program );

    // set up lighting for the scene
    setLighting( program );

    checkErrors( "display scene" );

    // draw the individual objects
    for( int obj = 0; obj < N_OBJECTS; ++obj ) {

        // are we texture mapping this object?
        if( map_obj[obj] ) {
            usingTextures = true;
        } else {
            usingTextures = false;
        }

        // set up texture/shading information
        setTextures( program, obj );

        checkErrors( "display object 1" );

        // select the correct rotation angles for this object
        GLfloat rotations[3] = { angles[obj], angles[obj], angles[obj] };

        // send all the transformation data
        switch( obj ) {
        case OBJ_QUAD:
            setTransforms( program, quad_s, rotations, quad_x );
            break;
        case OBJ_SPHERE:
            setTransforms( program, sphere_s, rotations, sphere_x );
            break;
        case OBJ_CYLINDER:  // FALL THROUGH
        case OBJ_DISCS:
            setTransforms( program, cyl_s, rotations, cyl_x );
            break;
        }

        checkErrors( "display object 2" );

        // draw it
        buffers[obj].selectBuffers( program,
            "vPosition", NULL, "vNormal", "vTexCoord" );

        checkErrors( "display object 3" );

        glDrawElements( GL_TRIANGLES, buffers[obj].numElements,
            GL_UNSIGNED_INT, (void *)0 );

        checkErrors( "display object 4" );
    }
}

///
// PUBLIC FUNCTIONS
///

///
// OpenGL initialization
///
bool appInit( int argc, char *argv[] )
{
    bool mapAll = true;
    char *endptr = NULL;

    // command-line arguments specify which objects to texture-map
    for( int i = 1; i < argc; ++i ) {
        long int n = strtol( argv[i], &endptr, 10 );
        if( endptr == argv[i] || *endptr != '\0' ) { // error?
            cerr << "bad object number '" << argv[1] << "' ignored" << endl;
        } else if( n < -1 || n >= N_OBJECTS ) { // bad object number?
            cerr << "bad object number '" << argv[1] << "' ignored" << endl;
        } else if( n == -1 ) {  // don't texture-map everything
            mapAll = false;
        } else {  // do texture-map this object
            mapAll = false;
            map_obj[n] = true;
        }
    }

    // if we're mapping everything, indicate that
    if( mapAll ) {
        for( int i = 0; i < N_OBJECTS; ++i ) {
            map_obj[i] = true;
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
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - "
	    << errorString(error) << endl;
	return( false );
    }

    glUseProgram( program );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // for each object, set its initial animation status and create it
    for( int obj = 0; obj < N_OBJECTS; ++obj ) {
        animating[obj] = false;
        angles[obj] = 0.0f;
        createShape( obj, *canvas );
    }

    // initialize all texture-related things
    initTextures();

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
        glfwPollEvents();
    }
}
