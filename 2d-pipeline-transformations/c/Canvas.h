///
//  Canvas.h
//
//  Prototypes for routines for manipulating a simple canvas
//  holding point information along with color data.
//
//  Modified for use in the midterm project.
//
//  Created by Warren R. Carithers 2016/09/23.
//  Based on a C++ version created by Joe Geigel.
//  Updates 2018/11/29, 2019/09/09, 2019/09/27 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
///

#ifndef _CANVAS_H_
#define _CANVAS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <stdbool.h>

#include "Vertex.h"
#include "Color.h"
#include "FloatVector.h"

///
// Simple canvas structure that allows for pixel-by-pixel rendering.
///

typedef struct st_Canvas {

    ///
    // canvas size information
    ///
    int width;
    int height;

    ///
    // point-related data
    ///
    
    // vertex locations
    FloatVector points;
    float *pointArray;

    // associated normal vectors
    FloatVector normals;
    float *normalArray;

    // associated (u,v) coordinates
    FloatVector uv;
    float *uvArray;

    // associated color data
    FloatVector colors;
    float *colorArray;

    // element count and connectivity data
    int numElements;
    GLuint *elemArray;

    ///
    // other Canvas defaults
    ///

    // current drawing color
    Color currentColor;

    // drawing depth
    float currentDepth;

    ///
    // Assignment-specific additions to Canvas
    ///

    ///
    // Data members
    ///

    ///
    // outline vs. polygon drawing variables
    //
    // these are public to facilitate use by the driver program
    ///
    bool drawingOutlines;
    int countNum;
    const int countNumLimit;
    int *outlineCounts;

} Canvas;

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas *canvasCreate( int w, int h );

///
// Destructor
//
// @param C canvas to destroy
///
void canvasDestroy( Canvas *C );

///
// Clears the canvas
//
// @param C The Canvas to use
///
void canvasClear( Canvas *C );

///
// Set the pixel Z coordinate
//
// @param d The depth to use
// @param C The Canvas to use
//
// @return  The old depth value
///
float canvasSetDepth( float d, Canvas *C );

///
// Set the current color
//
// @param color The desired color
// @param C The Canvas to use
//
// @return  The old color value
///
Color canvasSetColor( Color color, Canvas *C );

///
// Write a pixel using the current color
//
// @param v The pixel to be set
// @param C The Canvas to use
///
void canvasSetPixel( Vertex v, Canvas *C );

///
// Write a pixel using the specified color
//
// @param v The pixel to be set
// @param color The desired color
// @param C The Canvas to use
///
void canvasSetPixelColor( Vertex v, Color color, Canvas *C );

///
// Add a triangle to the current shape
//
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
// @param C The Canvas to be used
///
void canvasAddTriangle( Vertex p0, Vertex p1, Vertex p2, Canvas *C);

///
// Retrieve the array of element data from this Canvas
//
// @param C The Canvas to be used
///
GLuint *canvasGetElements( Canvas *C );

///
// Retrieve the array of vertex data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetVertices( Canvas *C );

///
// Retrieve the array of normal data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetNormals( Canvas *C );

///
// Retrieve the array of texture coordinate data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetUV( Canvas *C );

///
// Retrieve the array of color data from this Canvas
//
// @param C The Canvas to be used
///
float *canvasGetColors( Canvas *C );

///
// Retrieve the vertex count from this Canvas
//
// @param C The Canvas to be used
///
int canvasNumVertices( Canvas *C );

///
// Assignment-specific additions to Canvas
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
void canvasDrawOutline( int n, const Vertex v[], Canvas *C );

///
// Return the outline status of this object
//
// @param C The Canvas to be used
///
bool canvasIsOutline( Canvas *C );

#endif
