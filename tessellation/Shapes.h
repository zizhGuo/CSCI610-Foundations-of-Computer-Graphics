///
//  Shapes.h
//
//  Prototypes for tessellation routines to be supplied by students.
//
//  Author:  Warren R. Carithers 2017/04/01
//  Last updated 2019/03/27.
//
//  Contributor:  YOUR_NAME_HERE
///

#ifndef _SHAPES_H_
#define _SHAPES_H_

#include "Canvas.h"

///
// makeCube - Create a unit cube, centered at the origin, with a given number
// of subdivisions in each direction on each face.
//
// @param factor - number of equal subdivisons to be made in each 
//        direction along each face
//
// Can only use calls to C.addTriangle()
///
void makeCube( Canvas &C, int factor );

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
                   int discFactor, int heightFactor );

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
               int discFactor, int heightFactor );

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
void makeSphere( Canvas &C, float radius, int sliceFactor, int stackFactor );

#endif
