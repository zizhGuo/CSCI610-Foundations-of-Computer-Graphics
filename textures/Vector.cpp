///
//  Vector
//
//  Simple vector module implementation
//
//  Provides three-element vectors.
//
//  Author:  Warren R. Carithers
//  Date:    2019/11/18
//
//  This code can be compiled as either C or C++.
//
//  This file should not be modified by students.
///

#ifdef __cplusplus
// we cheat, and just use stdio here
#include <cstdio>
#include <cstdlib>
#include <cmath>
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#endif
#include <unistd.h>

#include "Vector.h"

#ifdef __cplusplus
using namespace std;
#endif

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
void vecPrint( const Vector vec, const char *msg, FILE *stream ) {

    if( msg ) fputs( msg, stream );

    fputs( "vector:", stream );

    for( size_t c = 0; c < N_VEC; ++c )
        fprintf( stream, "  %f", vec[c] );

    fputc( '\n', stream );
}

///
// Compute the dot product of two vectors
//
// @param v1  the first vector
// @param v2  the second vector
// @return    the dot product of the two vectors
///
float dot( const Vector v1, const Vector v2 ) {
    // a.b = ax*bx + ay*by + az*bz
    return(  v1[VX]*v2[VX] + v1[VY]*v2[VY] + v1[VZ]*v2[VZ] );
}

///
// Compute the cross product of two vectors
//
// @param result  the cross product of the two vectors
// @param v1      the first vector
// @param v2      the second vector
///
void cross( Vector result, const Vector v1, const Vector v2 ) {
    result[VX] = (v1[VY] * v2[VZ]) - (v1[VZ] * v2[VY]);  // ay*bz - az*by
    result[VY] = (v1[VZ] * v2[VX]) - (v1[VX] * v2[VZ]);  // az*bx - ax*bz
    result[VZ] = (v1[VX] * v2[VY]) - (v1[VY] * v2[VX]);  // ax*by - ay*bx
}


///
// Compute the magnitude of a vector
//
// Returns 0.0f for a zero vector.
//
// @param result  the magnitude of the vector
// @param vec     the vector
///
float mag( const Vector vec ) {
    // sum of the squares of the components
    float sum = vec[VX]*vec[VX] + vec[VY]*vec[VY] + vec[VZ]*vec[VZ];
    // 
    if( sum > 0.0f )
	// square root of that sum
        return( sqrt(sum) );
    else
        return( 0.0f );
}

///
// Normalize a vector
//
// Returns a zero vector if the original vector's length is zero
//
// @param result  the normalized vector
// @param vec     the original vector
///
void norm( Vector result, const Vector vec ) {
    float len = mag(vec);
    if( len != 0.0f ) {
        result[VX] = vec[VX] / len;
        result[VY] = vec[VY] / len;
        result[VZ] = vec[VZ] / len;
    } else {
        result[VX] = result[VY] = result[VZ] = 0.0f;
    }
}
