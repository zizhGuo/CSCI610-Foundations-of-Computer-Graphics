///
//  Cylinder.cpp
//
//  Code for drawing the cylinder.
//
//  Created by Warren R. Carithers 2019/11/23.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Zizhun Guo on 2019/12/12
///

#include <cmath>
#include <iostream>

#include "Canvas.h"

// pull in the data for the object
#include "Cylinder20.h"
#include "Cylinder.h"

using namespace std;

///
// convertSideUV() - convert the side vertex coodinate into UV coordinate
///
// Since the r = 0.5, h = 1.0 (based on the data in CylinderData.h)
// so for cylinder side, u = Phi / PI, v = y + (h / 2); h = 1.0
TexCoord convertSideUV(Vertex vertex){
	TexCoord t {0.00f, 0.00f};
	float PI {3.141592654};
	float Phi {atan(vertex.x / vertex.z)};

	float u {Phi/ PI};
	float v = {vertex.y + 0.5f};
	t.u  = u;
	t.v = 1.0f - v;	// manually invert the Y coordinate to fit the openGL 
                	// since y = 0 usually on the top of the image
                	// so it needs to be inverted.

	return t;
}

///
// convertDiscUV() - convert the disc vertex coodinate into UV coordinate
///
// Since the r = 0.5, h = 1.0 (based on the data in CylinderData.h)
// so for cylinder disc, u = x + (h / 2), v = z + (h/ 2); h = 1.0
TexCoord convertDiscUV(Vertex vertex){
	TexCoord t {0.00f, 0.00f};
	float PI {3.141592654};

	float u {(vertex.x + 0.5f)};
	float v {(vertex.z + 0.5f)};
	t.u  = u;
	t.v = 1.0f - v;	// manually invert the Y coordinate to fit the openGL 
                	// since y = 0 usually on the top of the image
                	// so it needs to be inverted.

	return t;
}

///
// makeCylinder() - create the cylinder body
///
void makeCylinder( Canvas &C )
{
    // Only use the vertices for the body itself
    for( int i = body.first; i <= body.last - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = cylinderElements[i];
        int point2 = cylinderElements[i + 1];
        int point3 = cylinderElements[i + 2];

        Vertex p1 = cylinderVertices[point1];
        Vertex p2 = cylinderVertices[point2];
        Vertex p3 = cylinderVertices[point3];

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
        C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );
         C.addTextureCoords(convertSideUV(p1), convertSideUV(p2), convertSideUV(p3));

	// You will need to add code here to determine texture
	// coordinates for each vertex, and will need to call the
	// addTextureCoords() function to add those coordinates
	// to the Canvas.
    }
}

///
// makeDiscs() - create the cylinder discs
///
void makeDiscs( Canvas &C )
{
    // Only use the vertices for the top and bottom discs

    for( int disc = 0; disc < 2; ++disc ) {

        // Select the starting and ending indices, and create the surface
        // normal for this disc.  For the top and bottom, the normals are
        // parallel to the Y axis.  Points on the disk all have Y == 0.5,
        // and those on the bottom have Y == -0.5.
	int first, last;
	Normal nn;

	if( disc == 0 ) { // bottom disc
	    first = bdisc.first;
	    last  = bdisc.last;
	    nn = (Normal) { 0.0f, -1.0f, 0.0f };
	} else {
	    first = tdisc.first;
	    last  = tdisc.last;
	    nn = (Normal) { 0.0f, 1.0f, 0.0f };
	}

        // Create the triangles
        for( int i = first; i <= last - 2; i += 3 ) {

            // Calculate the base indices of the three vertices
            int point1 = cylinderElements[i];
            int point2 = cylinderElements[i + 1];
            int point3 = cylinderElements[i + 2];

            Vertex p1 = cylinderVertices[point1];
            Vertex p2 = cylinderVertices[point2];
            Vertex p3 = cylinderVertices[point3];

	    // Add this triangle to the collection
            C.addTriangleWithNorms( p1, nn, p2, nn, p3, nn );
           
	    // You will need to add code here to determine texture
	    // coordinates for each vertex, and will need to call the
	    // addTextureCoords() function to add those coordinates
	    // to the Canvas.
            C.addTextureCoords(convertDiscUV(p1), convertDiscUV(p2), convertDiscUV(p3));
            
	}
    }
}
