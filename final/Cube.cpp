///
//  Cube.cpp
//
//  Code for drawing the cube.
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
#include "Cube20.h"
#include "Cube.h"

using namespace std;


///
// makeDiscs() - create the cylinder discs
///
void makeCube( Canvas &C )
{
    // Only use the vertices for the top and bottom discs

    for( int face = 0; face < 6; ++face ) {

        // Select the starting and ending indices, and create the surface
        // normal for this disc.  For the top and bottom, the normals are
        // parallel to the Y axis.  Points on the disk all have Y == 0.5,
        // and those on the bottom have Y == -0.5.
	int first, last;
	Normal nn;

	if( face == 0 ) // the right face
	{ 
	    first = rface.first;
	    last  = rface.last;
	    nn = (Normal) { 1.0f, 0.0f, 0.0f };
	}
	else if (face == 1) // the left face
	{
	    first = lface.first;
	    last  = lface.last;
	    nn = (Normal) { -1.0f, 0.0f, 0.0f };
	}
	else if (face == 2) // the top face
	{
	    first = tface.first;
	    last  = tface.last;
	    nn = (Normal) { 0.0f, 1.0f, 0.0f };		
	}
	else if (face == 3) // the bottom face
	{
	    first = bface.first;
	    last  = bface.last;
	    nn = (Normal) { 0.0f, -1.0f, 0.0f };		
	}
	else if (face == 4) // the front face
	{
	    first = fface.first;
	    last  = fface.last;
	    nn = (Normal) { 0.0f, 0.0f, 1.0f };		
	}
	else // the back face
	{
	    first = xface.first;
	    last  = xface.last;
	    nn = (Normal) { 0.0f, 0.0f, -1.0f };		
	}

        // Create the triangles
        for( int i = first; i <= last - 2; i += 3 ) {

            // Calculate the base indices of the three vertices
            int point1 = cubeElements[i];
            int point2 = cubeElements[i + 1];
            int point3 = cubeElements[i + 2];

            Vertex p1 = cubeVertices[point1];
            Vertex p2 = cubeVertices[point2];
            Vertex p3 = cubeVertices[point3];

	    // Add this triangle to the collection
            C.addTriangleWithNorms( p1, nn, p2, nn, p3, nn );
           
		}
    }
}
