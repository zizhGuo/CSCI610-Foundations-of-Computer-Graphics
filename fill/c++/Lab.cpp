///
//  Lab.cpp
//
//  Assignment-specific code for the "Hello, OpenGL!" assignment.
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

#include "Lab.h"

using namespace std;

///
// PRIVATE GLOBALS
///

///
// Object information
///

// ########### TEAPOT ###########
// Base
static Vertex g_base[] = {
    { 760.0f,  40.0f }, { 600.0f,  40.0f },
    { 620.0f,  60.0f }, { 740.0f,  60.0f }
};
static int n_base = sizeof(g_base) / sizeof(Vertex);
static Color c_base = { 1.0f, 0.0f, 0.0f };

// Body: right bottom triangle
static Vertex g_body1[] = {
    { 800.0f, 120.0f }, { 740.0f,  60.0f }, { 620.0f,  60.0f }
};
static int n_body1 = sizeof(g_body1) / sizeof(Vertex);
static Color c_body1 = { 0.9f, 0.0f, 0.0f };

// Body: midsection
static Vertex g_body2[] = {
    { 620.0f,  60.0f }, { 580.0f, 160.0f }, { 620.0f, 240.0f },
    { 740.0f, 240.0f }, { 800.0f, 120.0f }
};
static int n_body2 = sizeof(g_body2) / sizeof(Vertex);
static Color c_body2 = { 0.6f, 0.0f, 0.0f };

// Spout: lower triangle
static Vertex g_spout1[] = {
    { 620.0f,  60.0f }, { 560.0f, 100.0f }, { 500.0f, 180.0f }
};
static int n_spout1 = sizeof(g_spout1) / sizeof(Vertex);
static Color c_spout1 = { 0.8f, 0.0f, 0.0f };

// Spout: remainder
static Vertex g_spout2[] = {
    { 620.0f,  60.0f }, { 500.0f, 180.0f }, { 460.0f, 200.0f },
    { 520.0f, 200.0f }, { 580.0f, 160.0f }
};
static int n_spout2 = sizeof(g_spout2) / sizeof(Vertex);
static Color c_spout2 = { 0.7f, 0.0f, 0.0f };

// Handle
static Vertex g_handle[] = {
    { 800.0f, 120.0f }, { 840.0f, 160.0f }, { 855.0f, 200.0f },
    { 720.0f, 220.0f }, { 720.0f, 200.0f }, { 830.0f, 190.0f },
    { 825.0f, 165.0f }, { 780.0f, 120.0f }
};
static int n_handle = sizeof(g_handle) / sizeof(Vertex);
static Color c_handle = { 0.8f, 0.0f, 0.2f };

// Lid
static Vertex g_lid[] = {
    { 690.0f, 240.0f }, { 710.0f, 260.0f },
    { 650.0f, 260.0f }, { 670.0f, 240.0f }
};
static int n_lid = sizeof(g_lid) / sizeof(Vertex);
static Color c_lid = { 0.8f, 0.2f, 0.2f };

// ######## SHAPES #######
// Triangle
static Vertex g_tri[] = {
    { 460.0f, 220.0f }, { 490.0f, 280.0f }, { 420.0f, 280.0f }
};
static int n_tri = sizeof(g_tri) / sizeof(Vertex);
static Color c_tri = { 0.8f, 0.2f, 1.0f };

// Quad
static Vertex g_quad[] = {
    { 380.0f, 280.0f }, { 320.0f, 320.0f },
    { 360.0f, 380.0f }, { 420.0f, 340.0f }
};
static int n_quad = sizeof(g_quad) / sizeof(Vertex);
static Color c_quad = { 0.0f, 0.8f, 0.8f };

// Star: right half
static Vertex g_star1[] = {
    { 230.0f, 389.0f }, { 260.0f, 369.0f }, { 254.0f, 402.0f },
    { 278.0f, 425.0f }, { 245.0f, 430.0f }, { 230.0f, 460.0f },
    { 230.0f, 410.0f }
};
static int n_star1 = sizeof(g_star1) / sizeof(Vertex);
static Color c_star1 = { 1.0f, 0.60f, 0.0f };

// Star: left half
static Vertex g_star2[] = {
    { 230.0f, 460.0f }, { 216.0f, 430.0f }, { 183.0f, 425.0f },
    { 207.0f, 402.0f }, { 201.0f, 369.0f }, { 230.0f, 389.0f },
    { 230.0f, 410.0f }
};
static int n_star2 = sizeof(g_star2) / sizeof(Vertex);
static Color c_star2 = { 1.0f, 0.8f, 0.0f };

// ########## BORDERS ###############
// Bottom left corner: square
static Vertex g_sqbl[] = {
    {   0.0f,   0.0f }, {   0.0f,  20.0f },
    {  20.0f,  20.0f }, {  20.0f,   0.0f }
};
static int n_sqbl = sizeof(g_sqbl) / sizeof(Vertex);
static Color c_sqbl = { 0.0f, 0.4f, 0.4f };

// Top left corner: square
static Vertex g_sqtl[] = {
    {   0.0f, 580.0f }, {   0.0f, 599.0f },
    {  20.0f, 599.0f }, {  20.0f, 580.0f }
};
static int n_sqtl = sizeof(g_sqtl) / sizeof(Vertex);
static Color c_sqtl = { 0.0f, 0.2f, 0.8f };

// Top right corner: square
static Vertex g_sqtr[] = {
    { 899.0f, 599.0f }, { 899.0f, 580.0f },
    { 880.0f, 580.0f }, { 880.0f, 599.0f }
};
static int n_sqtr = sizeof(g_sqtr) / sizeof(Vertex);
static Color c_sqtr = { 0.0f, 0.6f, 0.2f };

// Bottom right corner: square
static Vertex g_sqbr[] = {
    { 899.0f,   0.0f }, { 899.0f,  20.0f },
    { 880.0f,  20.0f }, { 880.0f,   0.0f }
};
static int n_sqbr = sizeof(g_sqbr) / sizeof(Vertex);
static Color c_sqbr = { 0.0f, 0.6f, 0.0f };

// Bottom edge: quad
static Vertex g_qb[] = {
    {  20.0f,   0.0f }, {  20.0f,  20.0f },
    { 880.0f,  20.0f }, { 880.0f,   0.0f }
};
static int n_qb = sizeof(g_qb) / sizeof(Vertex);
static Color c_qb = { 0.2f, 0.8f, 0.0f };

// Left edge: quad
static Vertex g_ql[] = {
    {   0.0f,  20.0f }, {  20.0f,  20.0f },
    {  20.0f, 580.0f }, {   0.0f, 580.0f }
};
static int n_ql = sizeof(g_ql) / sizeof(Vertex);
static Color c_ql = { 0.0f, 0.2f, 1.0f };

// Top edge: upper triangle
static Vertex g_trt1[] = {
    {  20.0f, 580.0f }, {  20.0f, 599.0f }, { 880.0f, 599.0f }
};
static int n_trt1 = sizeof(g_trt1) / sizeof(Vertex);
static Color c_trt1 = { 0.0f, 0.4f, 1.0f };

// Top edge: lower triangle
static Vertex g_trt2[] = {
    {  20.0f, 580.0f }, { 880.0f, 580.0f }, { 880.0f, 599.0f }
};
static int n_trt2 = sizeof(g_trt2) / sizeof(Vertex);
static Color c_trt2 = { 0.0f, 0.6f, 1.0f };

// Right edge: lefthand triangle
static Vertex g_trr1[] = {
    { 880.0f, 580.0f }, { 899.0f, 580.0f }, { 880.0f,  20.0f }
};
static int n_trr1 = sizeof(g_trr1) / sizeof(Vertex);
static Color c_trr1 = { 0.0f, 1.0f, 0.4f };

// Right edge: righthand triangle
static Vertex g_trr2[] = {
    { 899.0f, 580.0f }, { 899.0f,  20.0f }, { 880.0f,  20.0f }
};
static int n_trr2 = sizeof(g_trr2) / sizeof(Vertex);
static Color c_trr2 = { 0.0f, 0.8f, 0.2f };

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 900;
int w_height = 600;

// drawing window title
const char *w_title = "Polygon Fill";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Canvas and Rasterizer
Canvas *C;
Rasterizer *R;

// buffers for our shapes
BufferSet shapes;

// shader program handle
GLuint program;

// names of our GLSL shader files (we assume GLSL 1.30)
const char *vshader = "v130.vert";
const char *fshader = "v130.frag";

///
// PRIVATE FUNCTIONS
///

///
// Create all the polygons and draw them using the Rasterizer
///
static void makePolygons( Rasterizer &R )
{
    // start with a clean canvas
    R.C.clear();

    // ########### TEAPOT ###########
    // Base
    R.C.setColor( c_base );
    R.drawPolygon( n_base, g_base );

    // Body: right bottom triangle
    R.C.setColor( c_body1 );
    R.drawPolygon( n_body1, g_body1 );

    // Body: midsection
    R.C.setColor( c_body2 );
    R.drawPolygon( n_body2, g_body2 );

    // Spout: lower triangle
    R.C.setColor( c_spout1 );
    R.drawPolygon( n_spout1, g_spout1 );

    // Spout: remainder
    R.C.setColor( c_spout2 );
    R.drawPolygon( n_spout2, g_spout2 );

    // Handle
    R.C.setColor( c_handle );
    R.drawPolygon( n_handle, g_handle );

    // Lid
    R.C.setColor( c_lid );
    R.drawPolygon( n_lid, g_lid );

    // ######## SHAPES #######
    // Triangle
    R.C.setColor( c_tri );
    R.drawPolygon( n_tri, g_tri );

    // Quad
    R.C.setColor( c_quad );
    R.drawPolygon( n_quad, g_quad );

    // Star: right half
    R.C.setColor( c_star1 );
    R.drawPolygon( n_star1, g_star1 );

    // Star: left half
    R.C.setColor( c_star2 );
    R.drawPolygon( n_star2, g_star2 );

    // ########## BORDERS ###############
    // Bottom left corner: square
    R.C.setColor( c_sqbl );
    R.drawPolygon( n_sqbl, g_sqbl );

    // Top left corner: square
    R.C.setColor( c_sqtl );
    R.drawPolygon( n_sqtl, g_sqtl );

    // Top right corner: square
    R.C.setColor( c_sqtr );
    R.drawPolygon( n_sqtr, g_sqtr );

    // Bottom right corner: square
    R.C.setColor( c_sqbr );
    R.drawPolygon( n_sqbr, g_sqbr );

    // Bottom edge: quad
    R.C.setColor( c_qb );
    R.drawPolygon( n_qb, g_qb );

    // Left edge: quad
    R.C.setColor( c_ql );
    R.drawPolygon( n_ql, g_ql );

    // Top edge: upper triangle
    R.C.setColor( c_trt1 );
    R.drawPolygon( n_trt1, g_trt1 );

    // Top edge: lower triangle
    R.C.setColor( c_trt2 );
    R.drawPolygon( n_trt2, g_trt2 );

    // Right edge: lefthand triangle
    R.C.setColor( c_trr1 );
    R.drawPolygon( n_trr1, g_trr1 );

    // Right edge: righthand triangle
    R.C.setColor( c_trr2 );
    R.drawPolygon( n_trr2, g_trr2 );
}

///
// Create the shapes we'll display
///
static void createImage( Rasterizer &R )
{
    // draw all our polygons
    makePolygons( R );

    // set up the OpenGL buffers
    shapes.createBuffers( R.C );
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
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose( window, 1 );
            break;
    }
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

    // bind the vertex and element buffers,
    // and set up the attribute variables
    shapes.selectBuffers( program, "vPosition", "vColor", NULL, NULL );

    // set up our scale factors for normalization
    GLuint sf = glGetUniformLocation( program, "sf" );
    glUniform2f( sf, 2.0f / (w_width - 1.0f), 2.0f / (w_height - 1.0f) );

    // draw the shapes
    glDrawElements( GL_POINTS, shapes.numElements, GL_UNSIGNED_INT, NULL );
}

///
// PUBLIC FUNCTIONS
///

///
// OpenGL initialization
///
bool labInit( void )
{
    // Create our Canvas "object"
    C = new Canvas( w_width, w_height );

    if( C == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
	return( false );
    }

    R = new Rasterizer( w_height, *C );

    if( R == NULL ) {
        cerr << "error - cannot create Rasterizer" << endl;
	delete C;
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
    glUseProgram( program );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createImage( *R );

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );

    return( true );
}

///
// Event loop for this assignment
///
void labLoop( void )
{
    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        display();
	glfwSwapBuffers( w_window );
	glfwWaitEvents();
    }
}
