///
//  Lab.cpp
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
#include <cstring>

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
#include "Pipeline.h"

#include "Lab.h"

using namespace std;

///
// PRIVATE GLOBALS
///

///
// Object information
///

// the triangle
static const Vertex triangle_v[] = {
    { 25.0f, 125.0f }, { 75.0f, 125.0f }, { 50.0f, 175.0f }
};
static const int triangle_nv = sizeof(triangle_v) / sizeof(Vertex);
static const Color triangle_c = { 0.25f, 0.00f, 0.74f, 1.00f };
static int triangle_id;

static const Vertex square_v[] = {
    { 125.0f, 125.0f }, { 175.0f, 125.0f },
    { 175.0f, 175.0f }, { 125.0f, 175.0f }
};
static const int square_nv = sizeof(square_v) / sizeof(Vertex);
static const Color square_c = { 0.00f, 0.91f, 0.08f, 1.00f };
static int square_id;

static const Vertex octagon_v[] = {
    { 25.0f, 25.0f }, { 35.0f, 15.0f }, { 55.0f, 15.0f }, { 75.0f, 25.0f },
    { 75.0f, 55.0f }, { 55.0f, 75.0f }, { 35.0f, 75.0f }, { 25.0f, 55.0f }
};
static const int octagon_nv = sizeof(octagon_v) / sizeof(Vertex);
static const Color octagon_c = { 0.98f, 0.00f, 0.48f, 1.00f };
static int octagon_id;

static const Vertex star_v[] = {
    { 150.0f, 90.0f }, { 140.0f, 65.0f }, { 110.0f, 65.0f }, { 140.0f, 40.0f },
    { 110.0f, 10.0f }, { 150.0f, 25.0f }, { 190.0f, 10.0f }, { 160.0f, 40.0f },
    { 190.0f, 65.0f }, { 160.0f, 65.0f }
};
static const int star_nv = sizeof(star_v) / sizeof(Vertex);
static const Color star_c = { 0.68f, 0.00f, 0.75f, 1.00f };
static int star_id;

static const Vertex rit1_v[] = {
    {  10.0f, 480.0f }, { 140.0f, 480.0f }, { 175.0f, 450.0f },
    { 175.0f, 430.0f }, { 140.0f, 370.0f }, {  90.0f, 370.0f },
    { 175.0f, 140.0f }, { 145.0f, 140.0f }, {  65.0f, 370.0f },
    {  35.0f, 370.0f }, {  35.0f, 140.0f }, {  10.0f, 140.0f }
};
static const int rit1_nv = sizeof(rit1_v) / sizeof(Vertex);
static const Color rit1_c = { 0.98f, 0.31f, 0.08f, 1.00f };
static int rit1_id;

static const Vertex rit2_v[] = {
    { 190.0f, 480.0f }, { 340.0f, 480.0f }, { 340.0f, 440.0f },
    { 280.0f, 440.0f }, { 280.0f, 180.0f }, { 340.0f, 180.0f },
    { 340.0f, 140.0f }, { 190.0f, 140.0f }, { 190.0f, 180.0f },
    { 250.0f, 180.0f }, { 250.0f, 440.0f }, { 190.0f, 440.0f }
};
static const int rit2_nv = sizeof(rit2_v) / sizeof(Vertex);
static const Color rit2_c = { 0.00f, 0.91f, 0.08f, 1.00f };
int rit2_id;

static const Vertex rit3_v[] = {
    { 360.0f, 480.0f }, { 480.0f, 480.0f }, { 480.0f, 440.0f },
    { 430.0f, 440.0f }, { 430.0f, 140.0f }, { 400.0f, 140.0f },
    { 400.0f, 440.0f }, { 360.0f, 440.0f }
};
static const int rit3_nv = sizeof(rit3_v) / sizeof(Vertex);
static const Color rit3_c = { 0.98f, 0.00f, 0.48f, 1.00f };
static int rit3_id;

static const Vertex rit4_v[] = {
    {  35.0f, 450.0f }, { 110.0f, 450.0f }, { 130.0f, 430.0f },
    { 110.0f, 410.0f }, {  35.0f, 410.0f }
};
static const int rit4_nv = sizeof(rit4_v) / sizeof(Vertex);
static const Color rit4_c = { 0.00f, 0.00f, 0.00f, 0.00f };
static int rit4_id;

// standard colors
static const Color background = { 0.00f, 0.00f, 0.00f, 1.00f };

// display management
static bool updateDisplay = true;

///
// PUBLIC GLOBALS
///

// Which image are we showing?
int displayNumber = 1;

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 500;
int w_height = 500;

// drawing window title
const char *w_title = "2D Pipeline";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Pipeline
Pipeline *pipeline;

// buffers for our shapes
BufferSet polyBuffers;

// shader program handle
GLuint program;

// names of our GLSL shader files (we assume GLSL 1.30)
const char *vshader = "v130.vert";
const char *fshader = "v130.frag";

///
// PRIVATE FUNCTIONS
///

///
// Create all the polygon objects
///
static void createImage( Pipeline &P )
{
    triangle_id = P.addPoly( triangle_nv, triangle_v );
    square_id   = P.addPoly( square_nv, square_v );
    octagon_id  = P.addPoly( octagon_nv, octagon_v );
    star_id     = P.addPoly( star_nv, star_v );
    rit1_id     = P.addPoly( rit1_nv, rit1_v );
    rit2_id     = P.addPoly( rit2_nv, rit2_v );
    rit3_id     = P.addPoly( rit3_nv, rit3_v );
    rit4_id     = P.addPoly( rit4_nv, rit4_v );
}

///
// Draw plain old polygons.
///
static void drawPolysNorm( Pipeline &P )
{
    ///
    // Draw a dark blue/purple triangle
    ///
    P.setColor( triangle_c );
    P.drawPoly( triangle_id );

    ///
    // Draw a green square
    ///
    P.setColor( square_c );
    P.drawPoly( square_id );

    ///
    // Draw a pink octagon
    ///
    P.setColor( octagon_c );
    P.drawPoly( octagon_id );

    ///
    // Draw a green star
    ///
    P.setColor( star_c );
    P.drawPoly( star_id );
}

///
// Draw the world transformed
///
static void drawPolysXform( Pipeline &P ) {

    ///
    // Draw a dark blue/purple triangle rotated
    ///
    P.clearTransform();
    P.rotate( -25.0f );
    P.setColor( triangle_c );
    P.drawPoly( triangle_id );

    ///
    // Draw a green square translated
    ///
    P.clearTransform();
    P.translate( 80.0f, 75.0f );
    P.setColor( square_c );
    P.drawPoly( square_id );

    ///
    // Draw a pink octagon scaled
    ///
    P.clearTransform();
    P.scale( 0.75f, 0.5f );
    P.setColor( octagon_c );
    P.drawPoly( octagon_id );

    ///
    // Draw a green star translated, scaled, rotated, then scaled back
    ///
    P.clearTransform();
    P.translate( 50.0f, 50.0f );
    P.scale( 2.0f, 2.0f );
    P.rotate( -10.0f );
    P.translate( -50.0f, 50.0f );
    P.setColor( star_c );
    P.drawPoly( star_id );
}

///
// Draw the RIT letters.  Colors have already been set.
///
static void drawLetters( Pipeline &P )
{
    ///
    // Draw with staggered translation
    ///

    // Draw the R
    P.clearTransform();
    P.translate( 15.0f, 0.0f );
    P.drawPoly( rit1_id );

    // Draw the I
    P.clearTransform();
    P.translate( 10.0f, 0.0f );
    P.drawPoly( rit2_id );

    // Draw the T
    P.clearTransform();
    P.translate( 5.0f, 0.0f );
    P.drawPoly( rit3_id );

    // Draw the hole in the R
    // MUST BE LAST, or anything after it isn't shown???
    P.clearTransform();
    P.translate( 15.0f, 0.0f );
    P.setColor( rit4_c );
    P.drawPoly( rit4_id );
}

///
// Draw the objects
///
static void drawObjects( Pipeline &P )
{
    ///
    // Set clear color to the background
    ///
    P.setColor( background );
    P.clear();
    P.clearTransform();

    switch( displayNumber % 4 ) {

    case 1: // plain old polygon test

        // default clip window
        P.setClipWindow( 0.0f, (float)(w_height-1),
                         0.0f, (float)(w_width-1) );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the polys
        drawPolysNorm( P );
        break;

    case 2: // clipping test

        // modified clip window
        P.setClipWindow( 35.0f, 175.0f, 35.0f, 165.0f );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the polys
        drawPolysNorm( P );
        break;

    case 3: // varying transformations

        // default clip window
        P.setClipWindow( 0.0f, (float)(w_height-1),
                         0.0f, (float)(w_width-1) );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the transformed polys
        drawPolysXform( P );
        break;

    case 0: // multiple varying viewports

        // default clipping
        P.setClipWindow( 0.0f, (float)(w_height-1),
                         0.0f, (float)(w_width-1) );

        // have some fun with the view port
        int wdiff = w_width / 3;
        int hdiff = w_height / 3;

        // will use xaspect and yaspect to
        // draw each row with a different ratio
        int xaspect = w_width / 3;
        int yaspect = w_height / 3;
        int x = 0;
        int y = 0;
        int i, j;

        for( i = 0; i < 3; i++ ) {
            // adjust yaspect
            yaspect = hdiff/(i+1);

            for( j = 0; j < 3; j++ ) {
                // let's play around with colors
                if( i == j ) // 1::1 ratios
                    P.setColor( rit1_c );
                else if( i < j ) // yaspect is smaller
                    P.setColor( rit2_c );
                else // i > j, xaspect larger
                    P.setColor( rit3_c );

                // adjust xaspect and shift to next column
                xaspect = wdiff/(j+1);
                P.setViewport( x, y, xaspect, yaspect );
                drawLetters( P );
                x += wdiff + 35;
            }
            // shift to next row, also add a little extra space
            // due to aspect ratio stuff making lots of blank canvas
            y += hdiff + 35;

            // change y aspect ratio
            xaspect = wdiff;

            // reset to left side of canvas
            x = 0;
        }
        break;
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

    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;

    case GLFW_KEY_P:  // basic polygons
    case GLFW_KEY_1:
        displayNumber = 1;  break;
        break;

    case GLFW_KEY_C:  // altered clipping
    case GLFW_KEY_2:
        displayNumber = 2;  break;
        break;

    case GLFW_KEY_T:  // object transformations
    case GLFW_KEY_3:
        displayNumber = 3;  break;
        break;

    case GLFW_KEY_V:  // altered viewports
    case GLFW_KEY_4:
        displayNumber = 4;  break;
        break;

    default:   // ignore everything else
        // return without triggering a redisplay
        return;
    }

    updateDisplay = true;
}

///
// Handle mouse clicks
///
static void mouse( GLFWwindow *window, int button, int action, int mods )
{
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
        displayNumber++;
        updateDisplay = true;
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

    // set up our scale factors for normalization
    GLuint sf = glGetUniformLocation( program, "sf" );
    glUniform2f( sf, 2.0f / (w_width - 1.0f), 2.0f / (w_height - 1.0f) );

    // draw the objects and create the buffers
    drawObjects( *pipeline );
    polyBuffers.createBuffers( *pipeline );

    // bind our buffers
    polyBuffers.selectBuffers( program, "vPosition", "vColor", NULL, NULL );

    if( pipeline->isOutline() ) {

        // draw the objects as separate line loops

        int skip = 0;
        for( int i = 0; i < pipeline->countNum; ++i ) {
            glDrawArrays( GL_LINE_LOOP, skip, pipeline->outlineCounts[i] );
            skip += pipeline->outlineCounts[i];
        }

    } else {

        // draw all the individual points
        glDrawElements( GL_POINTS, polyBuffers.numElements,
                        GL_UNSIGNED_INT, 0 );

    }
}

///
// PUBLIC FUNCTIONS
///

///
// OpenGL initialization
///
bool labInit( int argc, char *argv[] )
{
    // process the command-line argument if there is one
    if( argc > 1 ) {
        displayNumber = (int) strtol( argv[1], NULL, 10 );
        if( displayNumber < 1 || displayNumber > 4 ) {
            displayNumber = 1;
        }
    }

    // Create our Pipeline
    pipeline = new Pipeline( w_width, w_height );

    if( pipeline == NULL ) {
        cerr << "error - cannot create Pipeline" << endl;
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
    // glEnable( GL_CULL_FACE );
    // glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createImage( *pipeline );

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );
    glfwSetMouseButtonCallback( w_window, mouse );

    return( true );
}

///
// Event loop for this assignment
///
void labLoop( void )
{
    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        if( updateDisplay ) {
            updateDisplay = false;
            display();
        }
        glfwSwapBuffers( w_window );
        glfwWaitEvents();
    }
}
