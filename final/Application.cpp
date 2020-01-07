///
//  Application.cpp
//
//  Assignment-specific code for this assignment.
//
//  Created by Warren R. Carithers on 2019/09/09.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Zizhun Guo on 2019/12/12
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
#include "Cylinder.h"
#include "Sphere.h"
#include "Cone.h"
#include "Cube.h"

using namespace std;

///
// PRIVATE GLOBALS
///


// animation control
static float angles[N_OBJECTS];    // individual rotation angles


// names of our GLSL shader files
static const char *texture_vs    = "texture.vert";
static const char *texture_fs    = "texture.frag";
static const char *flat_vs    = "flat.vert";
static const char *flat_fs    = "flat.frag";
static const char *phong_vs    = "phong.vert";
static const char *phong_fs   = "phong.frag";

// buffers for our shapes
static BufferSet buffers[N_OBJECTS];


///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 1024;
int w_height = 669;

// drawing window title
const char *w_title = "Final Project";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Canvas
Canvas *canvas;

// shader programs
GLuint program, flat, phong, textures;

// current object
//int object;

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
        obj = SILO1;
    }

    // make the shape
    // SILO1: (Cylinder) The very left taller silo on the left hand side of image.
    // CAP1: (Sphere) The top semi-shpere for the silo1.
    // SILO2: (Cylinder) The second silo on the left.
    // CAP2: (Sphere) The top semi-shpere for the silo2.
    // BARN1: (Cylinder) The body of the barn.
    // CAP3: (Cone) The green cone top of the barn.
    // BARN2: (Cylinder) The body of the tower on the top of the barn.
    // CAP4: (Cone) The top of the tower top on the barn.
    // GROUND: (Cube) The grand ground.
    switch( obj ) {
        case SILO1:      makeCylinder( C );     break;
        case CAP1:       makeSphere( C );       break;
        case SILO2:      makeCylinder( C );     break;
        case CAP2:       makeSphere( C );       break;
        case BARN1:      makeCylinder( C );     break;
        case CAP3:       makeCone( C );         break;
        case BARN2:      makeCylinder( C );     break;
        case CAP4:       makeCone( C );         break;
        case GROUND:     makeCube ( C );        break;
        default:         makeCylinder( C );     break;
    }
    // create the necessary buffers
    buffers[obj].createBuffers( false, C );
}



///
// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    checkErrors( "display start" );

    // draw the individual objects
    for( int obj = 0; obj < N_OBJECTS; ++obj ) {
        
        // sets up the shader programs for each object
        switch( obj ) {
            case SILO1:      program = textures;        break;
            case CAP1:       program = phong;           break;
            case SILO2:      program = textures;        break;
            case CAP2:       program = phong;           break;
            case BARN1:      program = textures;        break;
            case CAP3:       program = flat;            break;
            case BARN2:      program = textures;        break;
            case CAP4:       program = phong;           break;
            case GROUND:     program = phong;           break;
        }

        // select the shader program
        glUseProgram( program );

        // set up projection parameters
        setFrustum( program );

        // set up the view transformation
        setCamera( program );

        checkErrors( "display scene" );

        // set up texture/shading information
        //setTextures( program, obj );

        checkErrors( "display object 1" );

        // select the correct rotation angles for this object
        GLfloat rotations[3] = { angles[obj], angles[obj], angles[obj] };

        // send all the transformation data
        
        switch( obj ) {
        case SILO1:
  
            // set up texture/shading information
            setTextures(program, SILO1);

            // set up model transformation
            setTransforms( program, silo1_s, rotations, silo1_x );
            break;
        case CAP1:
            // set up texture/shading information
            setLighting(program, CAP1);
            setTransforms( program, cap1_s, rotations, cap1_x );
            break;
        case SILO2:
            setTextures(program, SILO2);
            setTransforms( program, silo2_s, rotations, silo2_x );
            break;
        case CAP2:
            setLighting(program, CAP2);
            setTransforms( program, cap2_s, rotations, cap2_x );
            break;
        case BARN1:
            setTextures(program, BARN1);
            setTransforms( program, barn1_s, rotations, barn1_x );
            break;
        case CAP3:
            setLighting( program, CAP3 );;
            setTransforms( program, cap3_s, rotations, cap3_x );
            break;
        case BARN2:
            setTextures(program, BARN2);
            setTransforms( program, barn2_s, rotations, barn2_x );
            break;
        case CAP4:
            setLighting( program, CAP4 );;
            setTransforms( program, cap4_s, rotations, cap4_x );
            break;
        case GROUND:  // FALL THROUGH
            setLighting(program, GROUND);
            setTransforms( program, ground_s, rotations, ground_x );
            break;
        }
        // draw it
        buffers[obj].selectBuffers( program,
            "vPosition", NULL, "vNormal", "vTexCoord" );
        glDrawElements( GL_TRIANGLES, buffers[obj].numElements,
            GL_UNSIGNED_INT, (void *)0 );

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
    char *endptr = NULL;


    // Create our Canvas "object"
    canvas = new Canvas( w_width, w_height );

    if( canvas == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
	return( false );
    }

    // Load shaders and use the resulting shader program
    ShaderError error;

    // set up "texture" shader program
    textures = shaderSetup( texture_vs, texture_fs, &error );
    if( !textures ) {
        cerr << "Error setting up shaders - "
	    << errorString(error) << endl;
	return( false );
    }

    // set up "phong" shader program
    phong = shaderSetup( phong_vs, phong_fs, &error );
    if( !phong ) {
        cerr << "Error setting up shaders - "
        << errorString(error) << endl;
    return( false );
    }

    // set up "flat" shader program
    flat = shaderSetup( flat_vs, flat_fs, &error );
    if( !flat ) {
        cerr << "Error setting up shaders - "
        << errorString(error) << endl;
    return( false );
    }

    program = phong;  // -------------------------------------------------------------> set shader program as Phong

    glUseProgram( program );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // for each object, set its initial animation status and create it
    for( int obj = 0; obj < N_OBJECTS; ++obj ) {
        // animating[obj] = false;
        angles[obj] = 0.0f;
        createShape( obj, *canvas );
    }

    // sets current object
    object = SILO1;

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
{   glClearColor(0.457f, 0.715f, 0.898f, 1.0f);
    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        //animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( w_window );
        }
        glfwPollEvents();
    }
}
