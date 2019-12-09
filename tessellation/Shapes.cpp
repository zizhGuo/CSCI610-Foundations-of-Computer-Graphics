///
//  Shapes.cpp
//
//  Routines for tessellating a number of basic shapes
//
//  Students are to supply their implementations for the functions in
//  this file using the function addTriangle() to do the tessellation.
//
//  Author:  Warren R. Carithers 2017/04/01.
//  Last updated 2019/03/27.
//
//  Contributor:  Zizhun Guo
///

#include <cmath>

// Canvas.h pulls in all the OpenGL/GLFW/etc. header files for us
#include "Shapes.h"
#include "Canvas.h"
#include "Vertex.h"

///
// makeRecVertex - Create a Vertex based on given u and  v
//
// @param factor - the stable rectangle vertices and u v
//                  P[u, v] = (1 - v)R[u] + vQ[u]
///
Vertex makeRecVertex(float u, float v, Vertex p1, Vertex p2, Vertex p3, Vertex p4)
{
    float x = (1.0f - v) * ((1.0f - u) * p3.x + u * p4.x) + v * ((1.0f - u) * p1.x + u * p2.x);
    float y = (1.0f - v) * ((1.0f - u) * p3.y + u * p4.y) + v * ((1.0f - u) * p1.y + u * p2.y);
    float z = (1.0f - v) * ((1.0f - u) * p3.z + u * p4.z) + v * ((1.0f - u) * p1.z + u * p2.z);
    return Vertex{x, y, z, 1.0f};
}


///
// makeRecVertex - Tessellation on given square surface
//
// @param factor - the stable rectangle vertices and slice factor
//
///
void drawCubeFace(Canvas &C, int &factor, Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4 )
{
    // offset on given factor and stride for current location for the partitioned square
    float offset {1.0f / factor};
    float strideU = 0.0f;
    float strideV = 0.0f;

    for(int i {0}; i < factor; ++i) // column iteration
    {
        for(int j {0}; j < factor; ++j) // row iteration
        {
            // create two traiangles on current partitioned square
            Vertex vv1 = makeRecVertex(strideU, strideV, v1, v2, v3, v4);
            Vertex vv2 = makeRecVertex(strideU + offset, strideV + offset, v1, v2, v3, v4);
            Vertex vv3 = makeRecVertex(strideU, strideV + offset, v1, v2, v3, v4);
            Vertex vv4 = makeRecVertex(strideU + offset, strideV, v1, v2, v3, v4);
            C.addTriangle(vv1, vv2, vv3); // traiangle 1
            C.addTriangle(vv2, vv1, vv4); // traiangle 2
            strideU += offset;
        }
        strideU = 0.0f;
        strideV += offset;
    }
}
///
// makeCube - Create a unit cube, centered at the origin, with a given number
// of subdivisions in each direction on each face.
//
// @param factor - number of equal subdivisions to be made in each 
//        direction along each face
//
// Can only use calls to C.addTriangle()
///
void makeCube( Canvas &C, int factor )
{
    if( factor < 1 )
        factor = 1;

    // YOUR IMPLEMENTATION HERE
    // Specified 8 vertices and draw 6 faces
    Vertex v1 {-0.5f, -0.5f, 0.5f, 1.0f};
    Vertex v2 {0.5f, -0.5f, 0.5f, 1.0f};
    Vertex v3 {-0.5f, -0.5f,-0.5f, 1.0f};
    Vertex v4 {0.5f, -0.5f, -0.5f, 1.0f};
    Vertex v5 {-0.5f, 0.5f, 0.5f, 1.0f};
    Vertex v6 {0.5f, 0.5f, 0.5f, 1.0f};
    Vertex v7 {-0.5f, 0.5f,-0.5f, 1.0f};
    Vertex v8 {0.5f, 0.5f, -0.5f, 1.0f};
    drawCubeFace(C, factor, v1, v2, v3, v4);
    drawCubeFace(C, factor, v2, v6, v4, v8);
    drawCubeFace(C, factor, v6, v5, v8, v7);
    drawCubeFace(C, factor, v5, v1, v7, v3);
    drawCubeFace(C, factor, v1, v5, v2, v6);
    drawCubeFace(C, factor, v7, v3, v8, v4);
}

///
// makeDiskVertex - Create a Vertex based on given angle
//
// @param factor - angle within [0, 2PI], radius, and info of location: top/bottom
///
Vertex makeDiskVertex(float u, float radius, float tier)
{
    float x = cos(u) * radius;
    float y = sin(u) * radius;
    float z = tier;
    return Vertex{x, y, z, 1.0f};
}

///
// makeCylinder - Create polygons for a cylinder with unit height, centered at
// the origin, with separate number of radial subdivisions and height 
// subdivisions.
//
// @param radius - Radius of the base of the cylinder
// @param discFactor - number of subdivisions on the radial base
// @param heightFactor - number of subdivisions along the height
//
// Can only use calls to C.addTriangle()
///
void makeCylinder( Canvas &C, float radius,
                   int discFactor, int heightFactor )
{
    if( discFactor < 3 )
        discFactor = 3;

    if( heightFactor < 1 )
        heightFactor = 1;

    // YOUR IMPLEMENTATION HERE
    // offset on given discFactor and stride for current location on the same tier
    float strideDisk = 0.0f; 
    float offsetA = 2.0f * 3.14159f / discFactor;

    // offset on given heightFactor and stride for current height
    float strideSide = 0.0f;
    float offsetB = 1.0f / heightFactor;


    for (int i {0}; i < discFactor; ++i)
    {
        //Tessellation on disk top
        Vertex v1 = makeDiskVertex(strideDisk, radius, 0.5f);
        Vertex v2 = makeDiskVertex(strideDisk + offsetA, radius, 0.5f);
        C.addTriangle(Vertex{0.0f, 0.0f, 0.5f, 1.0f},v1, v2);

        //Tessellation on disk bottom
        Vertex v3 = makeDiskVertex(strideDisk, radius, -0.5f);
        Vertex v4 = makeDiskVertex(strideDisk + offsetA, radius, -0.5f);
        C.addTriangle(Vertex{0.0f, 0.0f, -0.5f, 1.0f},v4, v3);
        strideDisk += offsetA;

        //Tessellation on Cylinder side
        for(int j {0}; j < heightFactor; ++j)
        {
            Vertex vv1 = makeRecVertex(0.0f, strideSide, v1, v2, v3, v4);
            Vertex vv2 = makeRecVertex(1.0f, strideSide + offsetB, v1, v2, v3, v4);
            Vertex vv3 = makeRecVertex(0.0f, strideSide + offsetB, v1, v2, v3, v4);
            Vertex vv4 = makeRecVertex(1.0f, strideSide, v1, v2, v3, v4);
            C.addTriangle(vv1, vv2, vv3);
            C.addTriangle(vv2, vv1, vv4);
            strideSide += offsetB;
        }
        strideSide = 0.0f;
    }
}



///
// makeCone - Create polygons for a cone with unit height, centered at the
// origin, with separate number of radial subdivisions and height 
// subdivisions.
//
// @param radius - Radius of the base of the cone
// @param discFactor - number of subdivisions on the radial base
// @param heightFactor - number of subdivisions along the height
//
// Can only use calls to C.addTriangle()
///
void makeCone( Canvas &C, float radius,
               int discFactor, int heightFactor )
{
    if( discFactor < 3 )
        discFactor = 3;

    if( heightFactor < 1 )
        heightFactor = 1;

    // YOUR IMPLEMENTATION HERE
    // offset on given discFactor and stride for current location on the same tier
    float strideDisk = 0.0f;
    float offsetA = 2.0f * 3.14159f / discFactor;

    // offset on given heightFactor and stride for current height
    float strideSide = 0.0f;
    float offsetB = 1.0f / heightFactor;

    for (int i {0}; i < discFactor; ++i)
    {
        // disk bottom
        Vertex v1 = makeDiskVertex(strideDisk, radius, -0.5f);
        Vertex v2 = makeDiskVertex(strideDisk + offsetA, radius, -0.5f);
        C.addTriangle(Vertex{0.0f, 0.0f, -0.5f, 1.0f},v1, v2);
        strideDisk += offsetA;
        strideSide = 0.0f;

        //Cone side faces
        for(int j {0}; j < heightFactor; ++j)
        {
            if(j == heightFactor - 1) //Tessellation on first tier of small traiangles
            {
                float x1 = (1 - strideSide)*v1.x + strideSide * 0.0f;
                float y1 = (1 - strideSide)*v1.y + strideSide * 0.0f;
                float z1 = (1 - strideSide)*v1.z + strideSide * 0.5f;
                Vertex vv1 = Vertex{x1, y1, z1, 1.0f};

                float x2 = (1 - strideSide)*v2.x + strideSide * 0.0f;
                float y2 = (1 - strideSide)*v2.y + strideSide * 0.0f;
                float z2 = (1 - strideSide)*v2.z + strideSide * 0.5f;
                Vertex vv2 = Vertex{x2, y2, z2, 1.0f};

                C.addTriangle(vv2, vv1, Vertex{0.0f, 0.0f, 0.5f, 1.0f});
            }

            else //Tessellation on lower Rectangles (made by 4 vertices)
            {
                float x1 = (1.0f - strideSide)*v1.x + strideSide * 0.0f;
                float y1 = (1.0f - strideSide)*v1.y + strideSide * 0.0f;
                float z1 = (1.0f - strideSide)*v1.z + strideSide * 0.5f;
                Vertex vv1 = Vertex{x1, y1, z1, 1.0f};

                float x2 = (1.0f - strideSide)*v2.x + strideSide * 0.0f;
                float y2 = (1.0f - strideSide)*v2.y + strideSide * 0.0f;
                float z2 = (1.0f - strideSide)*v2.z + strideSide * 0.5f;
                Vertex vv2 = Vertex{x2, y2, z2, 1.0f};

                float x3 = (1.0f - strideSide - offsetB)*v1.x + (strideSide + offsetB) * 0.0f;
                float y3 = (1.0f - strideSide - offsetB)*v1.y + (strideSide + offsetB) * 0.0f;
                float z3 = (1.0f - strideSide - offsetB)*v1.z + (strideSide + offsetB)* 0.5f;
                Vertex vv3 = Vertex{x3, y3, z3, 1.0f};

                float x4 = (1.0f - strideSide - offsetB)*v2.x + (strideSide + offsetB) * 0.0f;
                float y4 = (1.0f - strideSide - offsetB)*v2.y + (strideSide + offsetB) * 0.0f;
                float z4 = (1.0f - strideSide - offsetB)*v2.z + (strideSide + offsetB)* 0.5f;
                Vertex vv4 = Vertex{x4, y4, z4, 1.0f};

                C.addTriangle(vv1, vv4, vv2);
                C.addTriangle(vv4, vv1, vv3);

                strideSide += offsetB;
            }

        }
    }
}

///
// makeCurvVertex - Create a Vertex based on given phi, theta and radius
//
// @param factor - theta within [0, 2PI], phi within [0, PI], radius
///
Vertex makeCurvVertex(float phi, float theta, float radius)
{
    float x = cos(theta) * sin(phi) * radius;
    float y = sin(theta) * sin(phi) * radius;
    float z = cos(phi) * radius;
    return Vertex{x, y, z, 1.0f};
}

///
// makeSphere - Create sphere of a given radius, centered at the origin, 
// using spherical coordinates with separate number of thetha and 
// phi subdivisions.
//
// @param radius - Radius of the sphere
// @param sliceFactor - number of subdivisions in the theta direction
// @param stackFactor - Number of subdivisions in the phi direction.
//
// Can only use calls to C.addTriangle()
///
void makeSphere( Canvas &C, float radius, int sliceFactor, int stackFactor )
{
    // IF DOING RECURSIVE SUBDIVISION:
    //   use only the "sliceFactor" parameter
    //   use a minimum value of 1 instead of 3
    //   add an 'else' clause to set a maximum value of 5

    if( sliceFactor < 3 )
        sliceFactor = 3;

    if( stackFactor < 3 )
        stackFactor = 3;

    // YOUR IMPLEMENTATION HERE
    // offset on given sliceFactor and stride for latitude
    float strideTheta = 0.0f;
    float offsetA = 2.0f * 3.14159f / sliceFactor;

    // offset on given stackFactor and stride for longitude
    float stridePhi = 0.0f;
    float offsetB = 3.14159f / stackFactor;

    // iteration on increasing size of Phi for drawing tier going down (longitude)
    for (int i {0}; i < stackFactor; ++i)
    {
        // iteration on increasing size of theta for the vertices on the same height (latitude)
        for(int j {0}; j < sliceFactor; ++j)
        {
            if(i == 0) // Tessallation on bottom case
            {
                Vertex v1 = makeCurvVertex(stridePhi + offsetB, strideTheta, radius);
                Vertex v2 = makeCurvVertex(stridePhi + offsetB, strideTheta + offsetA, radius);
                C.addTriangle(Vertex{0.0f, 0.0f, 0.5f, 1.0f}, v2, v1);
            }
            else if (i == stackFactor - 1) // Tessallation on bottom case
            {
                Vertex v1 = makeCurvVertex(stridePhi, strideTheta, radius);
                Vertex v2 = makeCurvVertex(stridePhi, strideTheta + offsetA, radius);
                C.addTriangle(Vertex{0.0f, 0.0f, -0.5f, 1.0f}, v1, v2);
            }
            else // Tessallation on middle regular cases
            {
                Vertex v1 = makeCurvVertex(stridePhi + offsetB, strideTheta, radius);
                Vertex v2 = makeCurvVertex(stridePhi + offsetB, strideTheta + offsetA, radius);
                Vertex v3 = makeCurvVertex(stridePhi, strideTheta, radius);
                Vertex v4 = makeCurvVertex(stridePhi, strideTheta + offsetA, radius);
                C.addTriangle(v1, v3, v2);
                C.addTriangle(v2, v3, v4);         
            }
            strideTheta += offsetA;
        }
        strideTheta = 0.0f;
        stridePhi += offsetB;
    }

}
