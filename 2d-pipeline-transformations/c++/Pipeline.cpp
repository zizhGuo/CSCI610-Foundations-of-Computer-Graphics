///
//  Pipeline.cpp
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/10/01 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Zizhun Guo
//                zg2808@cs.rit.edu
///

#include <iostream>
#include "Pipeline.h"
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


///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Pipeline::Pipeline( int w, int h ) 
    : Canvas(w,h), R(h, *this)
    // YOUR IMPLEMENTATION HERE if you need to add initializers
{
    // YOUR IMPLEMENTATION HERE if you need to modify the constructor
    
    // A STL::vector storing all Polygons to draw.
    Pipeline::polygons = vector<Polygon>();

    // Set all transformation matrices as identity matrix
    float fv[] {1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f};
    Pipeline::modelTrans = fMatrix(3, 3, fv);
    Pipeline::normClipTrans = fMatrix(3, 3, fv);
    Pipeline::viewPortTrans = fMatrix(3, 3, fv);
}

///
// addPoly - Add a polygon to the canvas.  This method does not draw
//           the polygon, but merely stores it for later drawing.
//           Drawing is initiated by the drawPoly() method.
//
//           Returns a unique integer id for the polygon.
//
// @param n - Number of vertices in polygon
// @param p - Array containing the vertices of the polygon to be added.
//
// @return a unique integer identifier for the polygon
///
int Pipeline::addPoly( int n, const Vertex p[] )
{
    // YOUR IMPLEMENTATION HERE
    
    Pipeline::polygons.push_back(Polygon(n, p));
    return Pipeline::counter++;
}

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn.
///
void Pipeline::drawPoly( int polyID )
{
    // YOUR IMPLEMENTATION HERE
    Polygon polygon {Pipeline::polygons.at(polyID)}; // fetch the polygon with given ID
    int size {polygon.size}; // get the number of vertices of the fetched polygon

    // Convert the current polygon vector to vertex array
    Vertex vs_origin[size];
    int count{0};
    for (auto i {polygon.vs.begin()}; i != polygon.vs.end(); ++i)
    {
        vs_origin[count] = *i;
        ++count;
    }    

    // Step 1: Model Transformation and Normalization
    // Convert vertex array into matrices array
    // Initialize temp matrices array
    fMatrix vms[size];
    for (int i {0}; i < size; ++i)
    {
        float fv[] {vs_origin[i].x, vs_origin[i].y, 1.0f};
        fMatrix vm(3, 1, fv);
        vms[i] = Pipeline::normClipTrans * Pipeline::modelTrans * vm;
    }
    // Convert the polygon matrices back to vertex array
    Vertex vs_normClip[size];
    for (int i {0}; i < size; ++i)
    {
        vs_normClip[i] = Vertex{vms[i][0][0], vms[i][1][0], 0.0f, 1.0f};
    }

    // Step 2: Clipping
    // Set an empty vertex array with double size of original array to 
    // storing the clipped polygon's data
    Vertex vs_viewPort[size * 2];
    int size_viewPort = 0;
    size_viewPort = clipPolygon(size, vs_normClip, vs_viewPort, Vertex{-1.0f, -1.0f}, Vertex{1.0f, 1.0f});

    // Step 3: Viewport Transformation
    // Convert vertex array into matrices array
    // Initialize temp matrices array 
    fMatrix vms_new[size_viewPort];
    for (int i {0}; i < size_viewPort; ++i)
    {
        float fv[] {vs_viewPort[i].x, vs_viewPort[i].y, 1.0f};
        fMatrix vm(3, 1, fv);
        vms_new[i] = Pipeline::viewPortTrans * vm;
    }
    // Convert the polygon matrices back to vertex array
    for (int i {0}; i < size_viewPort; ++i)
    {
        vs_viewPort[i] = Vertex{vms_new[i][0][0], vms_new[i][1][0], 0.0f, 1.0f};
    }

    // Step 4: Draw Polygon using Rasterizer's fillPolygon
    R.fillPolygon(size_viewPort, vs_viewPort);
}

///
// clearTransform - Set the current transformation to the identity matrix.
///
void Pipeline::clearTransform( void )
{
    // YOUR IMPLEMENTATION HERE
    float fv[] {1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f};
    fMatrix mf(3, 3, fv);
    Pipeline::modelTrans = mf; // reset to identity matrix
}

///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformation matrix.
//
// @param tx - Amount of translation in x.
// @param ty - Amount of translation in y.
///
void Pipeline::translate( float tx, float ty )
{
    // YOUR IMPLEMENTATION HERE
    float fv[] {1.0f, 0.0f, tx,
                0.0f, 1.0f, ty,
                0.0f, 0.0f, 1.0f};
    fMatrix mf(3, 3, fv);
    Pipeline::modelTrans = mf * Pipeline::modelTrans; // left multiplication matrix
}

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformation
//          matrix.
//
// @param degrees - Amount of rotation in degrees.
///
void Pipeline::rotate( float degrees )
{
    // YOUR IMPLEMENTATION HERE
    float rad {degrees * 3.1415f / 180};
    float fv[] {cos(rad), -sin(rad), 0.0f,
                sin(rad), cos(rad), 0.0f,
                0.0f, 0.0f, 1.0f};
    fMatrix mf(3, 3, fv);
    Pipeline::modelTrans = mf * Pipeline::modelTrans; // left multiplication matrix 
}

///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformation
//         matrix.
//
// @param sx - Amount of scaling in x.
// @param sy - Amount of scaling in y.
///
void Pipeline::scale( float sx, float sy )
{
    // YOUR IMPLEMENTATION HERE
    float fv[] {sx, 0.0f, 0.0f,
                0.0f, sy, 0.0f,
                0.0f, 0.0f, 1.0f};
    fMatrix mf(3, 3, fv);
    Pipeline::modelTrans = mf * Pipeline::modelTrans; // left multiplication matrix
}

///
// setClipWindow - Define the clip window.
//
// @param bottom - y coord of bottom edge of clip window (in world coords)
// @param top - y coord of top edge of clip window (in world coords)
// @param left - x coord of left edge of clip window (in world coords)
// @param right - x coord of right edge of clip window (in world coords)
///
void Pipeline::setClipWindow( float bottom, float top, float left, float right )
{
    // YOUR IMPLEMENTATION HERE
    float fv[] {2 / (right - left), 0.0f, (-2) * left / (right - left) - 1.0f,
                0.0f, 2 / (top - bottom), (-2) * bottom / (top - bottom) - 1.0f,
                0.0f, 0.0f, 1.0f};
    fMatrix mf(3, 3, fv);
    Pipeline::normClipTrans = mf; // reset normalization matrix
}

///
// setViewport - Define the viewport.
//
// @param x - x coord of lower left of view window (in screen coords)
// @param y - y coord of lower left of view window (in screen coords)
// @param w - width of view window (in pixels)
// @param h - height of view window (in pixels)
///
void Pipeline::setViewport( int x, int y, int w, int h )
{
    // YOUR IMPLEMENTATION HERE
    float fv[] {w / 2.0f, 0.0f, (x + x + w) / 2.0f,
                0.0f, h / 2.0f, (y + y + h) / 2.0f,
                0.0f, 0.0f, 1.0f};
    fMatrix mf(3, 3, fv);
    Pipeline::viewPortTrans = mf; // reset viewport matrix
}
