///
//  Vector.h
//
//  Simple vector module interface specification
//
//  Provides four-element vectors.
//
//  Author:  Warren R. Carithers
//  Date:    2019/11/18
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#ifndef _LINEAR_H_
#define _LINEAR_H_

#ifdef __cplusplus
// we cheat, and just use stdio here
#include <cstdio>
#include <cmath>
#else
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#endif

// number of components in vectors

#define	N_VEC	3

// More convenient names for vector components

#define	VX	0
#define	VY	1
#define	VZ	2

/////////////////////////////////
//
// Type declarations
//
/////////////////////////////////

///
// Our Vector type
///
typedef float Vector[N_VEC];

/////////////////////////////////
//
// Vector functions
//
/////////////////////////////////

///
// Print a vector
//
// Prints the vector in a standard format:
//
//    N elements:
//       v1   v2   ...   vN
//
// @param vec    the Vector to be printed
// @param msg    NULL, or identifying message to be printed
// @param stream where to print the Vector
///
void vecPrint( const Vector vec, const char *msg, FILE *stream );

///
// Compute the three-element dot product of two vectors
//
// @param v1  the first vector
// @param v2  the second vector
// @return    the dot product of the two vectors
///
float dot( const Vector v1, const Vector v2 );

///
// Compute the three-element cross product of two vectors
//
// @param result  the cross product of the two vectors
// @param v1      the first vector
// @param v2      the second vector
///
void cross( Vector result, const Vector v1, const Vector v2 );

///
// Compute the magnitude of a three-element vector
//
// @param result  the magnitude of the vector
// @param vec     the vector
///
float mag( const Vector vec );

///
// Normalize a three-element vector
//
// @param result  the normalized vector
// @param vec     the original vector
///
void norm( Vector result, const Vector vec );

#endif
