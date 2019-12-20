///
//  Shapes.h
//
//  Shape-drawing functions for the shading assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
///

#ifndef _SHAPES_H_
#define _SHAPES_H_

#include "Canvas.h"

// Macros for object and shading selection
#define	TORUS		0
#define	TEAPOT		1

#define	FLAT		0
#define	NOT_FLAT	1

///
// Create the triangle mesh for the specified object.
//
// @param object   which object is being drawn
// @param shading  what type of shading is desired
// @param C        which Canvas object to use
///
void makeShape( int object, int shading, Canvas &C );

#endif
