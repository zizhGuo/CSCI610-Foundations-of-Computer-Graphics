///
//  Sphere.cpp
//
//  Code for drawing the sphere.
//
//  Created by Warren R. Carithers 2019/11/23.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Zizhun Guo
///

#include <cmath>

#include "Canvas.h"

// pull in the data for the objects
#include "SphereData.h"
#include "Sphere.h"

using namespace std;

///
// convertSphereUV() - convert vertex coodinate into UV coordinate
///
// Since the r = 0.5, (based on the data in CylinderData.h)
// so for cylinder side, Phi keeps same, Theta = acos(y*2)
TexCoord convertSphereUV(Vertex vertex){
  TexCoord t {0.00f, 0.00f};
  float PI {3.141592654};
  float Phi {atan(vertex.x / vertex.z)};
  float Theta {acos(vertex.y * 2.0f)};

  float u {Phi/ PI};
  float v = {Theta / PI};
  t.u  = u;
  t.v = 1.0f - v; // manually invert the Y coordinate to fit the openGL 
                  // since y = 0 usually on the top of the image
                  // so it needs to be inverted.
  return t;
}

///
// makeSphere() - create a sphere object
///
void makeSphere( Canvas &C )
{
    for( int i = 0; i < sphereElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = sphereElements[i];
        int point2 = sphereElements[i + 1];
        int point3 = sphereElements[i + 2];

        Vertex p1 = sphereVertices[point1];
        Vertex p2 = sphereVertices[point2];
        Vertex p3 = sphereVertices[point3];

	// Calculate the surface normals
	// We just re-use the point indices because the normals
	// for points on a sphere are equal to the coordinates
	// of the points themselves.
        Normal n1 = { sphereVertices[point1].x,
                      sphereVertices[point1].y,
                      sphereVertices[point1].z };

        Normal n2 = { sphereVertices[point2].x,
                      sphereVertices[point2].y,
                      sphereVertices[point2].z };

        Normal n3 = { sphereVertices[point3].x,
                      sphereVertices[point3].y,
                      sphereVertices[point3].z };

        // Add triangle and vertex normals
        C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );
        C.addTextureCoords(convertSphereUV(p1), convertSphereUV(p2), convertSphereUV(p3));
	// You will need to add code here to determine texture
        // coordinates for each vertex, and will need to call the
        // addTextureCoords() function to add those coordinates
        // to the Canvas.
    }
}
