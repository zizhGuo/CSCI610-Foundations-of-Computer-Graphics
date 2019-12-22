///
//  Quad.cpp
//
//  Code for drawing the quad.
//
//  Created by Warren R. Carithers 2019/11/23.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
///

#include <cmath>

#include "Canvas.h"
#include "TexCoord.h"

// pull in the data for the objects
#include "QuadData.h"
#include "Quad.h"

using namespace std;

///
// makeQuad() - create a quad object
///
void makeQuad( Canvas &C )
{
    for( int i = 0; i < quadElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = quadElements[i];
        int point2 = quadElements[i + 1];
        int point3 = quadElements[i + 2];

        Vertex p1 = quadVertices[point1];
        Vertex p2 = quadVertices[point2];
        Vertex p3 = quadVertices[point3];

	// Calculate the surface normals for the three vertices
	Normal n1 = { quadNormals[0], quadNormals[1], quadNormals[2] };
	Normal n2 = n1;
	Normal n3 = n2;

	// Add this triangle to the collection
	C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );

        // Add the texture coordinates
        C.addTextureCoords( quadUV[point1], quadUV[point2], quadUV[point3] );
    }
}
