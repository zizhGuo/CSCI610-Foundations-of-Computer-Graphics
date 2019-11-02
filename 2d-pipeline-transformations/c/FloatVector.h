///
//  FloatVector.h
//
//  A replacement for the C++ STL vector<float> class.
//
//  Author:  Warren R. Carithers
//  Date:    2013/10/16 10:43:50
///

#ifndef _FLOATVECTOR_H_
#define _FLOATVECTOR_H_

#include <sys/types.h>

///
// define an alternative to the STL vector<float> class
///

typedef struct fv_s {
	size_t size;	// number of occupied slots in the vector
	size_t length;	// total number of slots in the vector
	size_t growth;	// how many slots to add when growing the vector
	float *vec;	// the vector itself
} FloatVector;


///
// Manipulation functions
///

///
// fvGrowthFactor -- set the growth factor for a FloatVector
//
// Returns the old growth factor.
///
size_t fvGrowthFactor( FloatVector *vec, size_t size );

///
// fvClear -- return a FloatVector to its original state
///
void fvClear( FloatVector *vec );

///
// fvPushBack -- add a float to the end of the FloatVector,
// automatically extending the vector if need be
//
// If the vector must be extended and the memory reallocation
// fails, this method will print an error message and exit.
///
void fvPushBack( FloatVector *vec, float coord );

///
// Pseudo-function: return the count of elements in a FloatVector
//
// Note: the argument is a pointer, for consistency with the
// other methods that operate on FloatVector variables.
///

#define fvSize(fvp)  ((fvp)->size)

#endif
