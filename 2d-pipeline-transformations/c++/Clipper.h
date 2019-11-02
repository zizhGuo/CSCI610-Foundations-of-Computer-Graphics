///
//  Clipper.h
//
//  Simple module that performs clipping
//
//  Created by Warren R. Carithers on 02/27/14.
//  Last updated 2019/02/16 by wrc.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2011 Rochester Institute of Technology. All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  Contributor:  Zizhun Guo
//                zg2808@cs.rit.edu
///

#ifndef _CLIPPER_H_
#define _CLIPPER_H_

#include "Vertex.h"
#include <list>
#include <algorithm>
#include <iostream>
///
// clipPolygon
//
// Clip the polygon with vertex count in and vertices inV against the
// rectangular clipping region specified by lower-left corner ll and
// upper-right corner ur. The resulting vertices are placed in outV.
//
// The routine should return the with the vertex count of polygon
// resulting from the clipping.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list
// @param outV  the outgoing vertex list
// @param ll    the lower-left corner of the clipping rectangle
// @param ur    the upper-right corner of the clipping rectangle
//
// @return number of vertices in the polygon resulting after clipping
//
///
int clipPolygon( int in, const Vertex inV[], Vertex outV[],
		 Vertex ll, Vertex ur );

///
// inside
//
// To decide whether the point P is inside boundary b1 -> b2.
// Can be used for any boundary defined by starting point b1 and ending point b2.
// Counter-clockwise order for clippin window.
//
// @param p  point P
// @param b1 starting point b1 of the boundary
// @param b2 starting point b2 of the boundary
//
// @return boolean var

bool inside(const Vertex &p, const Vertex &b1, const Vertex &b2);


//
// intersect
//
// polygon edge p -> s intersects with boundary b1 -> b2, modifying intersecting point i
//
// Two intersecting cases: 1. horizontal boundary; 2. parallel boundary.
//
// @param p one point end of the polygon edge
// @param s one point end of the polygon edge
// @param b1 starting point b1 of the boundary
// @param b2 starting point b2 of the boundary
// @param i intersecting point i
void intersect(Vertex p, Vertex s, Vertex b1, Vertex b2, Vertex &i);

//
// clipping
//
// This works for single-boundary clipping to the polygon
// traverse the polygon vertices to update its stats,
// based on four cases intersecting with side of region.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list (array)
// @param outV  the outgoing vertex list
// @param out   the number of vertices in the clipped polygon 
// @param b1 starting point b1 of the boundary
// @param b2 starting point b2 of the boundary

void SHPC(const Vertex inV[], list<Vertex> &outV, const int &in, int &out, const Vertex &b1, const Vertex &b2);

#endif
