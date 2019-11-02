///
//  Pipeline.h
//
//  Created by Warren R. Carithers on 2016/10/17.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/09/27 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
///

#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "Canvas.h"
#include "Vertex.h"
#include "Color.h"

///
// Simple wrapper class for midterm assignment
//
// Only methods in the implementation file whose bodies
// contain the comment
//
//    // YOUR IMPLEMENTATION HERE
//
// are to be modified by students; all of these are
// found at the end of the file.
///

typedef struct st_Pipeline {

    ///
    // our Canvas "object"
    ///
    Canvas *C;

} Pipeline;

///
// "Inherited" methods from Canvas
//
// These are all implemented as simple wrapper functions that invoke
// the corresponding Canvas function using the Canvas that
// is contained in the supplied Pipeline "object".
///

///
// Clears the canvas using the current color.
//
// @param P - the Pipeline to be used
///
void pipelineClear( Pipeline *P );

///
// Set the pixel Z coordinate
//
// @param d The depth to use
// @param P The Pipeline to use
//
// @return  The old depth value
///
float pipelineSetDepth( float d, Pipeline *P );

///
// Sets the current color.
//
// @param c - the new color
// @param P - the Pipeline to be used
///
void pipelineSetColor( Color c, Pipeline *P );

///
// Writes a pixel using the current color.
//
// @param v - The pixel to be set
// @param P - the Pipeline to be used
///
void pipelineSetPixel( Vertex v, Pipeline *P );

///
// Writes a pixel using the specified color.
//
// @param v - The pixel to be set
// @param color - the color to use
// @param P - the Pipeline to be used
///
void pipelineSetPixelColor( Vertex v, Color color, Pipeline *P );

///
// Draw the outline of a polygon.
//
// Use this drawOutline() method only if you were NOT able to
// create a working drawPolygon() Rasterizer routine of your own.
// This method will only draw the outline of the polygon.
//
// @param n The number of vertices in the polygon
// @param v the vertices that form the polygon
// @param P The Pipeline to use
///
void pipelineDrawOutline( int n, const Vertex v[], Pipeline *P );

///
// @param P The Pipeline to be used
///
GLuint *pipelineGetElements( Pipeline *P );

///
// retrieve the array of vertex data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetVertices( Pipeline *P );

///
// retrieve the array of normal data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetNormals( Pipeline *P );

///
// retrieve the array of (u,v) data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetUV( Pipeline *P );

///
// retrieve the array of color data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetColors( Pipeline *P );

///
// retrieve the vertex count from this Pipeline
//
// @param P The Pipeline to be used
///
int pipelineNumVertices( Pipeline *P );

///
// returns outline status of this object
//
// @param P The Pipeline to be used
///
bool pipelineIsOutline( Pipeline *P );

///
// Methods which extend Canvas by providing
// additional functionality related to the midterm
// project requirements.
//
// These methods are to be implemented by the student.
///

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Pipeline *pipelineCreate( int w, int h );

///
// Destructor
//
// @param P Pipeline to destroy
///
void pipelineDestroy( Pipeline *P );

///
// addPoly - Add a polygon to the canvas.  This method does not draw
//           the polygon, but merely stores it for later drawing.
//           Drawing is initiated by the drawPoly() method.
//
//           Returns a unique integer id for the polygon.
//
// @param n - Number of vertices in polygon
// @param p - Array of vertex entries defining the polygon to be added
// @param P - the Pipeline to be used
//
// @return a unique integer identifier for the polygon
///
int pipelineAddPoly( int n, const Vertex v[], Pipeline *P );

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn
// @param P - the Pipeline to be used
///
void pipelineDrawPoly( int polyID, Pipeline *P );

///
// clearTransform - Sets the current transformation to the
//                  identity matrix.
//
// @param P - the Pipeline to be used
///
void pipelineClearTransform( Pipeline *P );
    
///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformtion matrix.
//
// @param tx - Amount of translation in x
// @param ty - Amount of translation in y
// @param P - the Pipeline to be used
///
void pipelineTranslate( float tx, float ty, Pipeline *P );

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformtion
//          matrix.
//
// @param degrees - Amount of rotation in degrees
// @param P - the Pipeline to be used
///
void pipelineRotate( float degrees, Pipeline *P );
    
///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformtion
//         matrix.
//
// @param sx - Amount of scaling in x
// @param sy - Amount of scaling in y
// @param P - the Pipeline to be used
///
void pipelineScale( float sx, float sy, Pipeline *P );
    
///
// setClipWindow - Define the clip window.
//
// @param bottom - y coord of bottom edge of clip window (in world coords)
// @param top - y coord of top edge of clip window (in world coords)
// @param left - x coord of left edge of clip window (in world coords)
// @param right - x coord of right edge of clip window (in world coords)
// @param P - the Pipeline to be used
///
void pipelineSetClipWindow( float bottom, float top, float left, float right,
    Pipeline *P );
    
///
// setViewport - Define the viewport.
//
// @param x - x coord of lower left of view window (in screen coords)
// @param y - y coord of lower left of view window (in screen coords)
// @param w - width of view window (in world coords)
// @param h - width of view window (in world coords)
// @param P - the Pipeline to be used
///
void pipelineSetViewport( int x, int y, int w, int h, Pipeline *P );

#endif
