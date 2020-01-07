///
//  Cone.cpp
//
//  Code for drawing the cone.
//
//  Created by Zizhun Guo 2019/12/12.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//
///

#include <cmath>
#include <iostream>

#include "Canvas.h"

// pull in the data for the object
#include "Cone20.h"
#include "Cone.h"

using namespace std;


///
// makeCone() - create the cylinder body
///
void makeCone( Canvas &C )
{
    // Only use the vertices for the body itself
    for( int i = body_cone.first; i <= body_cone.last - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = coneElements[i];
        int point2 = coneElements[i + 1];
        int point3 = coneElements[i + 2];

        Vertex p1 = coneVertices[point1];
        Vertex p2 = coneVertices[point2];
        Vertex p3 = coneVertices[point3];

        // Calculate the normal vectors for each vertex
	Normal n1, n2, n3;

	// Normals on the body run from the axis to the vertex, and
	// are in the XZ plane; thus, for a vertex at (Px,Py,Pz), the
	// corresponding point on the axis is (0,Py,0), and the normal is
	// P - Axis, or just (Px,0,Pz).

	n1 = (Normal) { p1.x, 0.0f, p1.z };
	n2 = (Normal) { p2.x, 0.0f, p2.z };
	n3 = (Normal) { p3.x, 0.0f, p3.z };

	// Add this triangle to the collection
        C.addTriangle( p1, p2, p3);
    }
}

///
// makeDisc() - create the cone bottom disc
///
void makeDisc( Canvas &C )
{

        // Select the starting and ending indices, and create the surface
        // normal for this disc.  For the top and bottom, the normals are
        // parallel to the Y axis.  Points on the disk all have Y == 0.5,
        // and those on the bottom have Y == -0.5.
	int first, last;
	Normal nn;

    first = bdisc_cone.first;
    last  = bdisc_cone.last;
    nn = (Normal) { 0.0f, -1.0f, 0.0f };
\
        // Create the triangles
        for( int i = first; i <= last - 2; i += 3 ) {

            // Calculate the base indices of the three vertices
            int point1 = coneElements[i];
            int point2 = coneElements[i + 1];
            int point3 = coneElements[i + 2];

            Vertex p1 = coneVertices[point1];
            Vertex p2 = coneVertices[point2];
            Vertex p3 = coneVertices[point3];

	    // Add this triangle to the collection
            C.addTriangleWithNorms( p1, nn, p2, nn, p3, nn );     
	   }
}
