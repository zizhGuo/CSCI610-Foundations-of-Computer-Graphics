///
//  Canvas.c
//
//  Routines for adding points to create a new mesh.
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on a C++ version created by Joe Geigel.
//  Updates: 2018/11/29, 2019/09/07 by wrc.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
///

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Canvas.h includes all the OpenGL, GLFW, etc. headers for us
#include "Canvas.h"

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas *canvasCreate( int w, int h ) {
    Canvas *C = calloc( 1, sizeof(Canvas) );
    if( C != NULL ) {
        // This is how to initialize a const int field when the
        // memory is dynamically allocated.  The partial initializer
        // list puts 100 into countNumLimit, and leaves 0 in all the
        // other fields.
        Canvas init = { .countNumLimit = 100 };
        // next, we copy it into the "real" Canvas
        memcpy( C, &init, sizeof(Canvas) );
        C->width = w;
        C->height = h;
	C->currentDepth = -1.0f;
	C->currentColor.a = 1.0f;
        // all other fields contain zeroes
    }
    return( C );
}

///
// Destructor
//
// @param C canvas to destroy
///
void canvasDestroy( Canvas *C ) {
    if( !C )
        return;
    free( C );
}

///
// Clear the canvas
//
// @param C The Canvas to use
///
void canvasClear( Canvas *C )
{
    if( C->pointArray ) {
        free( C->pointArray );
        C->pointArray = 0;
    }
    if( C->normalArray ) {
        free( C->normalArray );
        C->normalArray = 0;
    }
    if( C->uvArray ) {
        free( C->uvArray );
        C->uvArray = 0;
    }
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }
    if( C->colorArray ) {
        free( C->colorArray );
        C->colorArray = 0;
    }
    fvClear( &(C->points) );
    fvClear( &(C->normals) );
    fvClear( &(C->uv) );
    fvClear( &(C->colors) );
    C->numElements = 0;
    C->currentColor = (Color) { 0.0f, 0.0f, 0.0f, 1.0f };
    
    // assignment-specific additions
    if( C->outlineCounts ) {
        free( C->outlineCounts );
        C->outlineCounts = 0;
    }
    C->outlineCounts = (int *) calloc( C->countNumLimit, sizeof(int) );
    C->drawingOutlines = false;
    C->countNum = 0;
}

///
// Set the pixel Z coordinate
//
// @param d The depth to use
// @param C The Canvas to use
//
// @return  The old depth value
///
float canvasSetDepth( float d, Canvas *C )
{
    float r = C->currentDepth;

    C->currentDepth = d;
    return( r );
}

///
// Set the current drawing color
//
// @param color The desired color
// @param C The Canvas to use
//
// @return  The old color value
///
Color canvasSetColor( Color color, Canvas *C )
{
    Color old = C->currentColor;

    C->currentColor = color;
    return( old );
}

///
// Set a pixel to the current drawing color
//
// @param v The pixel to be set
// @param C The Canvas to use
///
void canvasSetPixel( Vertex v, Canvas *C )
{
    fvPushBack( &(C->points), v.x );
    fvPushBack( &(C->points), v.y );
    fvPushBack( &(C->points), C->currentDepth );
    fvPushBack( &(C->points), 1.0 );

    fvPushBack( &(C->colors), C->currentColor.r );
    fvPushBack( &(C->colors), C->currentColor.g );
    fvPushBack( &(C->colors), C->currentColor.b );
    fvPushBack( &(C->colors), 1.0 );   // ignore the alpha channel

    C->numElements += 1;
}

///
// Set a pixel to the specified drawing color
//
// @param v The pixel to be set
// @param color The desired color
// @param C The Canvas to use
///
void canvasSetPixelColor( Vertex v, Color color, Canvas *C )
{
    fvPushBack( &(C->points), v.x );
    fvPushBack( &(C->points), v.y );
    fvPushBack( &(C->points), C->currentDepth );
    fvPushBack( &(C->points), 1.0 );

    fvPushBack( &(C->colors), color.r );
    fvPushBack( &(C->colors), color.g );
    fvPushBack( &(C->colors), color.b );
    fvPushBack( &(C->colors), 1.0 );   // ignore the alpha channel

    C->numElements += 1;
}

///
// Add a triangle to the current shape
//
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
// @param C The Canvas to be used
///
void canvasAddTriangle( Vertex p0, Vertex p1, Vertex p2, Canvas *C )
{
    fvPushBack( &(C->points), p0.x );
    fvPushBack( &(C->points), p0.y );
    fvPushBack( &(C->points), p0.z );
    fvPushBack( &(C->points), C->currentDepth );

    fvPushBack( &(C->points), p1.x );
    fvPushBack( &(C->points), p1.y );
    fvPushBack( &(C->points), p1.z );
    fvPushBack( &(C->points), C->currentDepth );

    fvPushBack( &(C->points), p2.x );
    fvPushBack( &(C->points), p2.y );
    fvPushBack( &(C->points), p2.z );
    fvPushBack( &(C->points), C->currentDepth );

    C->numElements += 3;  // three vertices per triangle
}

///
// Retrieve the array of element data from this Canvas
//
// @param C The Canvas to use
///
GLuint *canvasGetElements( Canvas *C )
{
    // delete the old element array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
	C->elemArray = 0;
    }

    int n = C->numElements;

    if( n > 0 ) {
        // create and fill a new element array
        C->elemArray = (GLuint *) malloc( n * sizeof(GLuint) );
        if( C->elemArray == 0 ) {
    	    perror( "element allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->elemArray[i] = i;
        }
    }

    return C->elemArray;
}

///
// Retrieve the array of vertex data from this Canvas
//
// @param C The Canvas to use
///
float *canvasGetVertices( Canvas *C )
{
    // delete the old point array if we have one
    if( C->pointArray ) {
        free( C->pointArray );
	C->pointArray = 0;
    }

    int n = fvSize( &(C->points) );

    if( n > 0 ) {
        // create and fill a new point array
        C->pointArray = (float *) malloc( n * sizeof(float) );
        if( C->pointArray == 0 ) {
    	    perror( "point allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->pointArray[i] = C->points.vec[i];
        }
    }

    return C->pointArray;
}

///
// Retrieve the array of normal data from this Canvas
//
// @param C The Canvas to use
///
float *canvasGetNormals( Canvas *C )
{
    // delete the old normal array if we have one
    if( C->normalArray ) {
        free( C->normalArray );
	C->normalArray = 0;
    }

    int n = fvSize( &(C->normals) );

    if( n > 0 ) {
        // create and fill a new normal array
        C->normalArray = (float *) malloc( n * sizeof(float) );
        if( C->normalArray == 0 ) {
    	    perror( "normal allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->normalArray[i] = C->normals.vec[i];
        }
    }

    return C->normalArray;
}

///
// Retrieve the array of texture coordinate data from this Canvas
//
// @param C The Canvas to use
///
float *canvasGetUV( Canvas *C )
{
    // delete the old texture coordinate array if we have one
    if( C->uvArray ) {
        free( C->uvArray );
	C->uvArray = 0;
    }

    int n = fvSize( &(C->uv) );

    if( n > 0 ) {
        // create and fill a new texture coordinate array
        C->uvArray = (float *) malloc( n * sizeof(float) );
        if( C->uvArray == 0 ) {
    	    perror( "uv allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->uvArray[i] = C->uv.vec[i];
        }
    }

    return C->uvArray;
}

///
// Retrieve the array of color data from this Canvas
//
// @param C The Canvas to use
///
float *canvasGetColors( Canvas *C )
{
    // delete the old color array if we have one
    if( C->colorArray ) {
        free( C->colorArray );
	C->colorArray = 0;
    }

    int n = fvSize( &(C->colors) );

    if( n > 0 ) {
        // create and fill a new color array
        C->colorArray = (float *) malloc( n * sizeof(float) );
        if( C->colorArray == 0 ) {
    	    perror( "color allocation failure" );
	    exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            C->colorArray[i] = C->colors.vec[i];
        }
    }

    return C->colorArray;
}

///
// Retrieve the vertex count from this Canvas
//
// @param C The Canvas to use
///
int canvasNumVertices( Canvas *C )
{
    return C->numElements;
}

///
// Assignment-specific additions
///

///
// Draw the outline of a polygon.
//
// Use this drawOutline() method only if you were NOT able to
// create a working drawPolygon() Rasterizer routine of your own.
// This method will only draw the outline of the polygon.
//
// @param n The number of vertices in the polygon
// @param v The array of vertices forming the polygon
// @param C The Canvas to use
///

void canvasDrawOutline( int n, const Vertex v[], Canvas *C )
{
    if( C->countNum >= C->countNumLimit ) {
        fprintf( stderr, "too many outlines to draw!\n" );
        return;
    }

    C->drawingOutlines = true;
    C->outlineCounts[(C->countNum)++] = n;

    for( int i = 0; i < n; i++ )
        canvasSetPixel( v[i], C );
}

///
// returns outline status of this object
///
bool canvasIsOutline( Canvas *C )
{
    return C->drawingOutlines;
}
