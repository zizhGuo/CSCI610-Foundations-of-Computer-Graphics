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
#include "Clipper.h"

using namespace std;

///
// PRIVATE GLOBALS
///

///
// Object information
///

// our clipping regions (LL and UR corners)

static Vertex regions[][2] = {
    { {  20, 120 }, {  70, 170 } },
    { {  30,  30 }, {  70,  80 } },
    { { 100,  34 }, { 130,  60 } },
    { {  90,  90 }, { 130, 120 } },
    { { 198, 198 }, { 276, 258 } },
    { { 221,  80 }, { 251, 101 } },
    { {  30, 200 }, {  90, 260 } }
};
static int n_regions = sizeof(regions) / (2 * sizeof(Vertex));

///
// our polygons (list of vertices)
///

// quads, clipping region 1
// shades of red

static Vertex quad1[] = {  // entirely inside the region
    {  35, 135 }, {  35, 155 }, {  55, 155 }, {  55, 135 }
};
static int quad1_nv = sizeof(quad1) / sizeof(Vertex);
static Color quad1c = { 1.00f, 0.00f, 0.00f, 1.00f };  // 0xFF0000

static Vertex quad2[] = {  // entirely outside the region
    { 130, 140 }, { 130, 170 }, {  90, 170 }, {  90, 140 }
};
static int quad2_nv = sizeof(quad2) / sizeof(Vertex);
static Color quad2c = { 1.00f, 0.40f, 0.40f, 1.00f };  // 0xFF6666

// quads, clipping region 2
// shades of blue

static Vertex quad3[] = {  // two vertices outside w/r/t the right edge
    {  64,  47 }, {  80,  47 }, {  80,  71 }, {  64,  71 }
};
static int quad3_nv = sizeof(quad3) / sizeof(Vertex);
static Color quad3c = { 0.00f, 0.45f, 0.60f, 1.00f };  // 0x0073FF

static Vertex quad4[] = {  // two vertices outside w/r/t the left edge
    {  20,  60 }, {  50,  60 }, {  50,  50 }, {  20,  50 }
};
static int quad4_nv = sizeof(quad4) / sizeof(Vertex);
static Color quad4c = { 0.00f, 0.60f, 0.80f, 1.00f };  // 0x0099CC

static Vertex quad5[] = {  // two vertices outside w/r/t the top edge
    {  50,  70 }, {  60,  70 }, {  60, 100 }, {  50, 100 }
};
static int quad5_nv = sizeof(quad5) / sizeof(Vertex);
static Color quad5c = { 0.00f, 0.75f, 1.00f, 1.00f };  // 0x00BFFF

static Vertex quad6[] = {  // two vertices outside w/r/t the bottom edge
    {  40,  40 }, {  60,  40 }, {  60,  20 }, {  40,  20 }
};
static int quad6_nv = sizeof(quad6) / sizeof(Vertex);
static Color quad6c = { 0.40f, 0.85f, 1.00f, 1.00f };  // 0x66D9FF

static Vertex pent1[] = {
    {  90,  20 }, { 100,  10 }, { 120,  20 }, { 110,  50 }, {  90,  40 }
};
static int pent1_nv = sizeof(pent1) / sizeof(Vertex);
static Color pent1c = { 0.72f, 0.44f, 0.86f, 1.00f };  // 0xB86FDC

static Vertex pent2[] = {
    { 120,  40 }, { 160,  60 }, { 180,  50 }, { 170,  80 }, { 125,  65 }
};
static int pent2_nv = sizeof(pent2) / sizeof(Vertex);
static Color pent2c = { 0.84f, 0.68f, 0.92f, 1.00f };  // 0xD6ADEB

static Vertex hept1[] = {
    { 120,  80 }, { 140,  80 }, { 160,  90 }, { 160, 110 },
    { 140, 120 }, { 120, 110 }, { 110, 100 }
};
static int hept1_nv = sizeof(hept1) / sizeof(Vertex);
static Color hept1c = { 0.70f, 0.70f, 0.70f, 1.00f };  // 0xB3B3B3

static Vertex nona1[] = {
    { 190,  56 }, { 230,  68 }, { 247,  56 }, { 269,  71 }, { 284, 104 },
    { 251, 122 }, { 233, 110 }, { 212, 119 }, { 203,  95 }
};
static int nona1_nv = sizeof(nona1) / sizeof(Vertex);
static Color nona1c = { 0.87f, 0.72f, 0.53f, 1.00f };  // 0xDEB887

static Vertex deca1[] = {
    { 177, 156 }, { 222, 188 }, { 267, 156 }, { 250, 207 }, { 294, 240 },
    { 240, 240 }, { 222, 294 }, { 204, 240 }, { 150, 240 }, { 194, 207 }
};
static int deca1_nv = sizeof(deca1) / sizeof(Vertex);
static Color deca1c = { 1.00f, 0.65f, 0.00f, 1.00f };  // 0xFFA500

// triangles, clipping region 7
// shades of green

static Vertex tri1[] = {
    {  60, 180 }, {  70, 210 }, {  50, 210 }
};
static int tri1_nv = sizeof(tri1) / sizeof(Vertex);
static Color tri1c = { 0.00f, 0.60f, 0.00f, 1.00f };  // 0x009900

static Vertex tri2[] = {
    {  80, 220 }, { 110, 230 }, {  80, 240 }
};
static int tri2_nv = sizeof(tri2) / sizeof(Vertex);
static Color tri2c = { 0.00f, 0.80f, 0.00f, 1.00f };  // 0x00CC00

static Vertex tri3[] = {
    {  50, 250 }, {  70, 250 }, {  60, 280 }
};
static int tri3_nv = sizeof(tri3) / sizeof(Vertex);
static Color tri3c = { 0.00f, 1.00f, 0.00f, 1.00f };  // 0x00FF00

static Vertex tri4[] = {
    {  10, 230 }, {  40, 220 }, {  40, 240 }
};
static int tri4_nv = sizeof(tri4) / sizeof(Vertex);
static Color tri4c = { 0.40f, 1.00f, 0.40f, 1.00f };  // 0x66FF66

// basic colors
static Color white = { 1.0f, 1.0f, 1.0f, 1.0f };

// count of vertices in each clipped polygon
#define MAX_POLYS       20
int nv[MAX_POLYS];

///
// PUBLIC GLOBALS
///

///
// Drawing-related variables
///

// dimensions of the drawing window
int w_width  = 300;
int w_height = 300;

// drawing window title
const char *w_title = "Polygon Clipping";

// GL context we're using (we assume 3.0, for GLSL 1.30)
int gl_maj = 3;
int gl_min = 0;

// our GLFWwindow
GLFWwindow *w_window;

// our Canvas
Canvas *C;

// variables related to drawing the clipping regions
BufferSet clipBuffers;

// variables related to drawing the original polygons
BufferSet polyOutlineBuffers;

// variables related to drawing the resulting polygons
BufferSet clippedPolyBuffers;

// shader program handle
GLuint program;

// names of our GLSL shader files (we assume GLSL 1.30)
const char *vshader = "v130.vert";
const char *fshader = "v130.frag";

///
// PRIVATE FUNCTIONS
///

///
// Support for individual tests
///

static unsigned int which = 0;

#define N_TESTS    15
#define TEST_ALL   0xffffffff
#define IFTEST(n)  if( which & (1 << ((n)-1)) )

///
// iterate through argv converting numeric parameters
// and setting test bits accordingly
//
// invoke as:   set_tests( argc, argv );
// check as:    IFTEST(n) { .... }
///
static void set_tests( const int argc, char *argv[] ) {
    char *endptr;

    for( int i = 1; i < argc; ++i ) {
        long int n = strtol( argv[i], &endptr, 10 );
        if( endptr == argv[i] || *endptr != '\0' ) { // no/bad conversion
	    cerr << "bad test # '" << argv[i] << "' ignored" << endl;
        } else {
            if( n >= 1 && n <= N_TESTS ) {
                which |= (1 << (n-1));
            } else {
	        cerr << "bad test # '" << argv[i] << "' ignored" << endl;
            }
        }
    }

    if( which == 0 ) {
        which = TEST_ALL;
    }

}

///
// Support function that draws clip regions as line loops
///
static void drawClipRegion( Vertex ll, Vertex ur, Canvas &C ) {
    C.setPixel( (Vertex) { ll.x, ll.y } );  // LL
    C.setPixel( (Vertex) { ur.x, ll.y } );  // LR
    C.setPixel( (Vertex) { ur.x, ur.y } );  // UR
    C.setPixel( (Vertex) { ll.x, ur.y } );  // UL
}

///
// Draw all the clipping rectangles
///
static void makeClipOutlines( Canvas &C ) {

    // we draw the clipping regions as white rectangles.
    // all vertices are put into one vertex buffer, and
    // we use glDrawArrays() instead of glDrawElements()
    // to draw them as line loops

    for( int i = 0; i < n_regions; ++i ) {
        drawClipRegion( regions[i][0], regions[i][1], C );
    }
}

///
// Draw a polygon
///
static void drawPolygon( Vertex v[], int nv, Canvas &C ) {

    // just put the vertices into the vertex buffer, in order

    for( int i = 0; i < nv; ++i ) {
        C.setPixel( v[i] );
    }
}

///
// Create the polygon outlines
///
static void makePolygonOutlines( Canvas &C ) {

    // here, we draw the original polygons; these
    // will be rendered using line loops

    // region 1
IFTEST( 1 ) {
    C.setColor( quad1c );
    drawPolygon( quad1, quad1_nv, C );
}
IFTEST( 2 ) {
    C.setColor( quad2c );
    drawPolygon( quad2, quad2_nv, C );
}

    // region 2
IFTEST( 3 ) {
    C.setColor( quad3c );
    drawPolygon( quad3, quad3_nv, C );
}
IFTEST( 4 ) {
    C.setColor( quad4c );
    drawPolygon( quad4, quad4_nv, C );
}
IFTEST( 5 ) {
    C.setColor( quad5c );
    drawPolygon( quad5, quad5_nv, C );
}
IFTEST( 6 ) {
    C.setColor( quad6c );
    drawPolygon( quad6, quad6_nv, C );
}

    // region 3
IFTEST( 7 ) {
    C.setColor( pent1c );
    drawPolygon( pent1, pent1_nv, C );
}
IFTEST( 8 ) {
    C.setColor( pent2c );
    drawPolygon( pent2, pent2_nv, C );
}

    // region 4
IFTEST( 9 ) {
    C.setColor( hept1c );
    drawPolygon( hept1, hept1_nv, C );
}

    // region 5
IFTEST( 10 ) {
    C.setColor( nona1c );
    drawPolygon( nona1, nona1_nv, C );
}

    // region 6
IFTEST( 11 ) {
    C.setColor( deca1c );
    drawPolygon( deca1, deca1_nv, C );
}

    // region 7
IFTEST( 12 ) {
    C.setColor( tri1c );
    drawPolygon( tri1, tri1_nv, C );
}
IFTEST( 13 ) {
    C.setColor( tri2c );
    drawPolygon( tri2, tri2_nv, C );
}
IFTEST( 14 ) {
    C.setColor( tri3c );
    drawPolygon( tri3, tri3_nv, C );
}
IFTEST( 15 ) {
    C.setColor( tri4c );
    drawPolygon( tri4, tri4_nv, C );
}
}

///
// Create the filled polygons
///
static void makePolygons( Canvas &C ) {
    // temporary vertex array
    Vertex tmp[50];

    ///
    // region one:  quads, shades of red
    ///

IFTEST( 1 ) {
    // entirely inside
    C.setColor( quad1c );
    nv[0] = clipPolygon( quad1_nv, quad1, tmp, regions[0][0], regions[0][1] );
    if( nv[0] > 0 ) {
        drawPolygon( tmp, nv[0], C );
    }
}

IFTEST( 2 ) {
    // entirely outside
    C.setColor( quad2c );
    nv[1] = clipPolygon( quad2_nv, quad2, tmp, regions[0][0], regions[0][1] );
    // shouldn't draw anything!
    if( nv[1] > 0 ) {
        drawPolygon( tmp, nv[1], C );
    }
}

    ///
    // region two:  more quads, shades of blue
    ///

IFTEST( 3 ) {
    // two vertices outside w/r/t the right edge
    C.setColor( quad3c );
    nv[2] = clipPolygon( quad3_nv, quad3, tmp, regions[1][0], regions[1][1] );
    if( nv[2] > 0 ) {
        drawPolygon( tmp, nv[2], C );
    }
}

IFTEST( 4 ) {
    // two vertices outside w/r/t the left edge
    C.setColor( quad4c );
    nv[3] = clipPolygon( quad4_nv, quad4, tmp, regions[1][0], regions[1][1] );
    if( nv[3] > 0 ) {
        drawPolygon( tmp, nv[3], C );
    }
}

IFTEST( 5 ) {
    // two vertices outside w/r/t the top edge
    C.setColor( quad5c );
    nv[4] = clipPolygon( quad5_nv, quad5, tmp, regions[1][0], regions[1][1] );
    if( nv[4] > 0 ) {
        drawPolygon( tmp, nv[4], C );
    }
}

IFTEST( 6 ) {
    // two vertices outside w/r/t the bottom edge
    C.setColor( quad6c );
    nv[5] = clipPolygon( quad6_nv, quad6, tmp, regions[1][0], regions[1][1] );
    if( nv[5] > 0 ) {
        drawPolygon( tmp, nv[5], C );
    }
}

    ///
    // region three: pentagons, outside w/r/t two edges
    ///

IFTEST( 7 ) {
    // outside w/r/t the left and bottom edges
    C.setColor( pent1c );
    nv[6] = clipPolygon( pent1_nv, pent1, tmp, regions[2][0], regions[2][1] );
    if( nv[6] > 0 ) {
        drawPolygon( tmp, nv[6], C );
    }
}

IFTEST( 8 ) {
    // outside w/r/t the top and right edges
    C.setColor( pent2c );
    nv[7] = clipPolygon( pent2_nv, pent2, tmp, regions[2][0], regions[2][1] );
    if( nv[7] > 0 ) {
        drawPolygon( tmp, nv[7], C );
    }
}

    ///
    // region four:  outside on top, right, and bottom
    ///

IFTEST( 9 ) {
    C.setColor( hept1c );
    nv[8] = clipPolygon( hept1_nv, hept1, tmp, regions[3][0], regions[3][1] );
    if( nv[8] > 0 ) {
        drawPolygon( tmp, nv[8], C );
    }
}

    ///
    // region five:  surrounds the clip region
    ///

IFTEST( 10 ) {
    C.setColor( nona1c );
    nv[9] = clipPolygon( nona1_nv, nona1, tmp, regions[5][0], regions[5][1] );
    if( nv[9] > 0 ) {
        drawPolygon( tmp, nv[9], C );
    }
}

    ///
    // region six:  outside on all four edges
    ///

IFTEST( 11 ) {
    C.setColor( deca1c );
    nv[10] = clipPolygon( deca1_nv, deca1, tmp, regions[4][0], regions[4][1] );
    if( nv[10] > 0 ) {
        drawPolygon( tmp, nv[10], C );
    }
}

    ///
    // region seven: outside w/r/t one edge
    ///

IFTEST( 12 ) {
    C.setColor( tri1c );
    nv[11] = clipPolygon( tri1_nv, tri1, tmp, regions[6][0], regions[6][1] );
    if( nv[11] > 0 ) {
        drawPolygon( tmp, nv[11], C );
    }
}

IFTEST( 13 ) {
    C.setColor( tri2c );
    nv[12] = clipPolygon( tri2_nv, tri2, tmp, regions[6][0], regions[6][1] );
    if( nv[12] > 0 ) {
        drawPolygon( tmp, nv[12], C );
    }
}

IFTEST( 14 ) {
    C.setColor( tri3c );
    nv[13] = clipPolygon( tri3_nv, tri3, tmp, regions[6][0], regions[6][1] );
    if( nv[13] > 0 ) {
        drawPolygon( tmp, nv[13], C );
    }
}

IFTEST( 15 ) {
    C.setColor( tri4c );
    nv[14] = clipPolygon( tri4_nv, tri4, tmp, regions[6][0], regions[6][1] );
    if( nv[14] > 0 ) {
        drawPolygon( tmp, nv[14], C );
    }
}
}

///
// Create all our objects
///
static void createImage( Canvas &C )
{
    // start with a clean canvas
    C.clear();

    // first, create the clipping region buffers
    //
    // start by putting all the vertices for all
    // the regions into a single set of buffers

    // draw all of them in white
    C.setColor( white );
    makeClipOutlines( C );

    // collect the vertex, element, and color data for these
    clipBuffers.createBuffers( C );

    // next, do the polygon outlines
    C.clear();
    makePolygonOutlines( C );
    polyOutlineBuffers.createBuffers( C );

    // finally, do the polygons
    C.clear();
    makePolygons( C );
    clippedPolyBuffers.createBuffers( C );

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

    // set up our scale factors for normalization
    GLuint sf = glGetUniformLocation( program, "sf" );
    glUniform2f( sf, 2.0f / (w_width - 1.0f), 2.0f / (w_height - 1.0f) );

    ///
    // first, draw the polygon outlines
    ///

    // bind our buffers
    polyOutlineBuffers.selectBuffers( program, "vPosition", "vColor",
        NULL, NULL );

    // draw our shapes
    int skip = 0;
    IFTEST( 1){glDrawArrays( GL_LINE_LOOP, skip, quad1_nv ); skip += quad1_nv;}
    IFTEST( 2){glDrawArrays( GL_LINE_LOOP, skip, quad2_nv ); skip += quad2_nv;}
    IFTEST( 3){glDrawArrays( GL_LINE_LOOP, skip, quad3_nv ); skip += quad3_nv;}
    IFTEST( 4){glDrawArrays( GL_LINE_LOOP, skip, quad4_nv ); skip += quad4_nv;}
    IFTEST( 5){glDrawArrays( GL_LINE_LOOP, skip, quad5_nv ); skip += quad5_nv;}
    IFTEST( 6){glDrawArrays( GL_LINE_LOOP, skip, quad6_nv ); skip += quad6_nv;}
    IFTEST( 7){glDrawArrays( GL_LINE_LOOP, skip, pent1_nv ); skip += pent1_nv;}
    IFTEST( 8){glDrawArrays( GL_LINE_LOOP, skip, pent2_nv ); skip += pent2_nv;}
    IFTEST( 9){glDrawArrays( GL_LINE_LOOP, skip, hept1_nv ); skip += hept1_nv;}
    IFTEST(10){glDrawArrays( GL_LINE_LOOP, skip, nona1_nv ); skip += nona1_nv;}
    IFTEST(11){glDrawArrays( GL_LINE_LOOP, skip, deca1_nv ); skip += deca1_nv;}
    IFTEST(12){glDrawArrays( GL_LINE_LOOP, skip, tri1_nv  ); skip += tri1_nv;}
    IFTEST(13){glDrawArrays( GL_LINE_LOOP, skip, tri2_nv  ); skip += tri2_nv;}
    IFTEST(14){glDrawArrays( GL_LINE_LOOP, skip, tri3_nv  ); skip += tri3_nv;}
    IFTEST(15){glDrawArrays( GL_LINE_LOOP, skip, tri4_nv  ); skip += tri4_nv;}

    ///
    // next, draw the clipped polygons
    ///

    // bind our buffers
    clippedPolyBuffers.selectBuffers( program, "vPosition", "vColor",
        NULL, NULL );

    // draw our shapes
    //
    // be sure to only draw what's there
    skip = 0;
    for( int i = 0; i < MAX_POLYS; ++i ) {
        if( nv[i] ) {
            glDrawArrays( GL_TRIANGLE_FAN, skip, nv[i] ); skip += nv[i];
        }
    }

    ///
    // finally, draw the clip region outlines (we do these
    // last so that they show up on top of everything else)
    ///

    // bind our buffers
    clipBuffers.selectBuffers( program, "vPosition", "vColor", NULL, NULL );

    // draw our shapes
    glDrawArrays( GL_LINE_LOOP,  0, 4 );
    glDrawArrays( GL_LINE_LOOP,  4, 4 );
    glDrawArrays( GL_LINE_LOOP,  8, 4 );
    glDrawArrays( GL_LINE_LOOP, 12, 4 );
    glDrawArrays( GL_LINE_LOOP, 16, 4 );
    glDrawArrays( GL_LINE_LOOP, 20, 4 );
    glDrawArrays( GL_LINE_LOOP, 24, 4 );
}

///
// PUBLIC FUNCTIONS
///

///
// OpenGL initialization
///
bool labInit( int argc, char *argv[] )
{
    // process command-line test specifications
    if( argc > 0 ) {
        set_tests( argc, argv );
    }

    // Create our Canvas "object"
    C = new Canvas( w_width, w_height );

    if( C == NULL ) {
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

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_CULL_FACE );
    // glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createImage( *C );

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
