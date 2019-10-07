//
//  Canvas.h
//
//  Prototypes for routines for manipulating a simple canvas
//  holding point information along with color data.
//
//  Created by Warren R. Carithers 2016/09/30.
//  Based on a C++ version created by Joe Geigel.
//  Updates 2018/11/29, 2019/09/09 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#ifndef _CANVAS_H_
#define _CANVAS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Color.h"
#include "Vertex.h"

using namespace std;

#include <vector>

///
// Simple canvas class that allows for pixel-by-pixel rendering.
///

class Canvas {

    ///
    // canvas size information
    ///
    int width;
    int height;

    ///
    // point-related data
    ///

    // vertex locations
    vector<float> points;
    float *pointArray;

    // associated normal vectors
    vector<float> normals;
    float *normalArray;

    // associated (u,v) coordinates
    vector<float> uv;
    float *uvArray;

    // associated color data
    vector<float> colors;
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

public:
    ///
    // Constructor
    //
    // @param w width of canvas
    // @param h height of canvas
    ///
    Canvas( int w, int h );

    ///
    // Destructor
    ///
    ~Canvas( void );

    ///
    // clears the canvas
    ///
    void clear( void );

    ///
    // Set the pixel Z coordinate
    //
    // @param d The depth to use
    //
    // @return  The old depth value
    ///
    float setDepth( float d );

    ///
    // Set the current drawing color
    //
    // @param color The desired color
    //
    // @return  The old color value
    ///
    Color setColor( Color color );

    ///
    // Write a pixel using the current color
    //
    // @param v The pixel to be set
    ///
    void setPixel( Vertex v );

    ///
    // Write a pixel using the specified drawing color
    //
    // @param v The pixel to be set
    // @param color The desired color
    ///
    void setPixelColor( Vertex v, Color color );

    ///
    // Add a triangle to the current shape
    //
    // @param p0 first triangle vertex
    // @param p1 second triangle vertex
    // @param p2 final triangle vertex
    ///
    void addTriangle( Vertex p0, Vertex p1, Vertex p2 );

    ///
    // Retrieve the array of element data from this Canvas
    ///
    GLuint *getElements( void );

    ///
    // Retrieve the array of vertex data from this Canvas
    ///
    float *getVertices( void );

    ///
    // Retrieve the array of normal data from this Canvas
    ///
    float *getNormals( void );

    ///
    // Retrieve the array of (u,v) data from this Canvas
    ///
    float *getUV( void );

    ///
    // Retrieve the array of color data from this Canvas
    ///
    float *getColors( void );

    ///
    // Retrieve the vertex count from this Canvas
    ///
    int numVertices( void );

};

#endif
