///
//  Pipeline.h
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/10/01 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Zizhun Guo
//                zg2808@cs.rit.edu
///

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "Canvas.h"
#include "Vertex.h"

#include "Rasterizer.h" // for reusing Lab1 Polygon Fill
#include "Clipper.h" // for reusing Lab2 Polygon Clip

#include "cmatrix" // for matrix manipulation
#include <cmath>  // for radian and trigonometric manipulation
#include <vector> // for containing polygons data 

using namespace std;

typedef techsoft::matrix<float> fMatrix;


///
// Simple class for storing original Polygon data 
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students.
///
class Polygon
{
    public:
        int size; // The number of vertices
        
        vector<Vertex> vs; // The STL::vector storing current polygon's vertices

        Polygon(int num, const Vertex vertices[]): size(num) // constructor
        {
            vs = vector<Vertex>();
            for(int i {0}; i < size; ++i) 
            {
                vs.push_back(Vertex{vertices[i].x, vertices[i].y, vertices[i].z, vertices[i].w});
            }
        }
};



///
// Simple wrapper class for midterm assignment
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students.
///

class Pipeline : public Canvas {

public:
    Rasterizer R; // The Rasterizer utilized for Polygon Fill

    vector<Polygon> polygons; // polygons container
    
    int counter{0}; // used for addPoly return 
    
    fMatrix modelTrans; // The Model Transformation Matrix
  
    fMatrix normClipTrans; // The Normalization Transformation Matrix
    
    fMatrix viewPortTrans; // THe ViewPortTransformation Matrix


    ///
    // Constructor
    //
    // @param w width of canvas
    // @param h height of canvas
    ///
    Pipeline(int w, int h);

    ///
    // addPoly - Add a polygon to the canvas.  This method does not draw
    //           the polygon, but merely stores it for later drawing.
    //           Drawing is initiated by the drawPoly() method.
    //
    // @param n - Number of vertices in polygon
    // @param p - Array of Vertex entries defining the polygon to be added
    //
    // @return a unique integer identifier for the polygon
    ///
    int addPoly( int n, const Vertex p[] );

    ///
    // drawPoly - Draw the polygon with the given id.  The polygon should
    //            be drawn after applying the current transformation to
    //            the vertices of the polygon.
    //
    // @param polyID - the ID of the polygon to be drawn.
    ///
    void drawPoly( int polyID );

    ///
    //
    // clearTransform - Set the current transformation to the identity matrix.
    //
    ///
    void clearTransform( void );

    ///
    // translate - Add a translation to the current transformation by
    //             premultiplying the appropriate translation matrix to
    //             the current transformation matrix.
    //
    // @param tx - Amount of translation in x.
    // @param ty - Amount of translation in y.
    //
    ///
    void translate( float tx, float ty );

    ///
    // rotate - Add a rotation to the current transformation by premultiplying
    //          the appropriate rotation matrix to the current transformation
    //          matrix.
    //
    // @param degrees - Amount of rotation in degrees.
    ///
    void rotate( float degrees );

    ///
    // scale - Add a scale to the current transformation by premultiplying
    //         the appropriate scaling matrix to the current transformation
    //         matrix.
    //
    // @param sx - Amount of scaling in x.
    // @param sy - Amount of scaling in y.
    ///
    void scale( float sx, float sy );

    ///
    // setClipWindow - Define the clip window.
    //
    // @param bottom - y coord of bottom edge of clip window (in world coords)
    // @param top - y coord of top edge of clip window (in world coords)
    // @param left - x coord of left edge of clip window (in world coords)
    // @param right - x coord of right edge of clip window (in world coords)
    ///
    void setClipWindow( float bottom, float top, float left, float right );

    ///
    // setViewport - Define the viewport.
    //
    // @param x - x coord of lower left of view window (in screen coords)
    // @param y - y coord of lower left of view window (in screen coords)
    // @param w - width of view window (in world coords)
    // @param h - width of view window (in world coords)
    ///
    void setViewport( int x, int y, int w, int h );

};

#endif
