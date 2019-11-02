///
//  Pipeline.c
//
//  Created by Warren R. Carithers on 2016/10/17.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/09/27 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
///

#include <stdlib.h>
#include <stdio.h>

#include "Pipeline.h"

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
void pipelineClear( Pipeline *P )
{
    canvasClear( P->C );
}

///
// Set the pixel Z coordinate
//
// @param d The depth to use
// @param P The Pipeline to use
//
// @return  The old depth value
///
float pipelineSetDepth( float d, Pipeline *P )
{
    return( canvasSetDepth(d,P->C) );
}

///
// Sets the current color.
//
// @param c - the new color
// @param P - the Pipeline to be used
///
void pipelineSetColor( Color c, Pipeline *P )
{
    canvasSetColor( c, P->C );
}

///
// Writes a pixel using the current color.
//
// @param v - The vertex to be set
// @param P - the Pipeline to be used
///
void pipelineSetPixel( Vertex v, Pipeline *P )
{
    canvasSetPixel( v, P->C );
}

///
// Draw the outline of a polygon.
//
// Use this drawOutline() method only if you were NOT able to
// create a working drawPolygon() Rasterizer routine of your own.
// This method will only draw the outline of the polygon.
//
// @param n The number of vertices in the polygon
// @param v The vertices that form the polygon
// @param P The Pipeline to use
///
void pipelineDrawOutline( int n, const Vertex v[], Pipeline *P ) {
    canvasDrawOutline( n, v, P->C );
}

///
// retrieve the array of element data from this Pipeline
//
// @param P The Pipeline to be used
///
GLuint *pipelineGetElements( Pipeline *P ) {
    return canvasGetElements( P->C );
}

///
// retrieve the array of vertex data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetVertices( Pipeline *P ) {
    return canvasGetVertices( P->C );
}

///
// retrieve the array of normal data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetNormals( Pipeline *P ) {
    return canvasGetNormals( P->C );
}

///
// retrieve the array of (u,v) data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetUV( Pipeline *P ) {
    return canvasGetUV( P->C );
}

///
// retrieve the array of color data from this Pipeline
//
// @param P The Pipeline to be used
///
float *pipelineGetColors( Pipeline *P ) {
    return canvasGetColors( P->C );
}

///
// retrieve the vertex count from this Pipeline
//
// @param P The Pipeline to be used
///
int pipelineNumVertices( Pipeline *P ) {
    return canvasNumVertices( P->C );
}

///
// returns outline status of this object
//
// @param P The Pipeline to be used
///
bool pipelineIsOutline( Pipeline *P ) {
    return canvasIsOutline( P->C );
}

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
// @param C - the Canvas to be used by the Pipeline
///
Pipeline *pipelineCreate( int w, int h )
{
    Pipeline *new = (Pipeline *) calloc( 1, sizeof(Pipeline) );
    if( new != NULL ) {
        new->C = canvasCreate( w, h );
	if( new->C == NULL ) {
	    fprintf( stderr, "error - cannot create Canvas for pipeline\n" );
	    free( new );
	    new = NULL;
	}
    }

    // YOUR IMPLEMENTATION HERE if you need to modify the constructor

    return( new );
}

///
// Destructor
//
// @param P - the Pipeline to destroy
///
void pipelineDestroy( Pipeline *P )
{
    if( !P ) 
    	return;
    if( P->C )
        canvasDestroy( P->C );
    free( P );
}

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
int pipelineAddPoly( int n, const Vertex p[], Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE

    // REMEMBER TO RETURN A UNIQUE ID FOR THE POLYGON
    return 0;
}

///
// drawPoly - Draw the polygon with the given id.  The polygon should
//            be drawn after applying the current transformation to
//            the vertices of the polygon.
//
// @param polyID - the ID of the polygon to be drawn
// @param P - the Pipeline to be used
///
void pipelineDrawPoly( int polyID, Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}

///
// clearTransform - Set the current transformation to the identity matrix.
//
// @param P - the Pipeline to be used
///
void pipelineClearTransform( Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}
    
///
// translate - Add a translation to the current transformation by
//             premultiplying the appropriate translation matrix to
//             the current transformtion matrix.
//
// @param tx - Amount of translation in x
// @param ty - Amount of translation in y
// @param P - the Pipeline to be used
///
void pipelineTranslate( float tx, float ty, Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}

///
// rotate - Add a rotation to the current transformation by premultiplying
//          the appropriate rotation matrix to the current transformtion
//          matrix.
//
// @param degrees - Amount of rotation in degrees
// @param P - the Pipeline to be used
///
void pipelineRotate( float degrees, Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}
    
///
// scale - Add a scale to the current transformation by premultiplying
//         the appropriate scaling matrix to the current transformtion
//         matrix.
//
// @param sx - Amount of scaling in x
// @param sy - Amount of scaling in y
// @param P - the Pipeline to be used
///
void pipelineScale( float sx, float sy, Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}
    
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
    Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}
    
///
// setViewport - Define the viewport.
//
// @param x - x coord of lower left of view window (in screen coords)
// @param y - y coord of lower left of view window (in screen coords)
// @param w - width of view window (in pixels)
// @param h - width of view window (in pixels)
// @param P - the Pipeline to be used
///
void pipelineSetViewport( int x, int y, int w, int h, Pipeline *P )
{
    // YOUR IMPLEMENTATION HERE
}
