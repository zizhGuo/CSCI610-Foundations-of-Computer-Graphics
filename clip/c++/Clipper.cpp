///
//  Clipper
//
//  Simple module that performs clipping
//
//  Created by Warren R. Carithers on 01/15/14.
//  Updates: 2019/02/16, 2019/09/17 by wrc.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  This code can be compiled as either C or C++.
//
//  Contributor: Zizhun Guo 
//  zg2808@cs.rit.edu
///

#include <list>
#include <algorithm>
#include <iostream>


#ifdef __cplusplus
using namespace std;
#else
#include <stdbool.h>
#endif

#include "Vertex.h"
#include "Clipper.h"


///
// inside
//
// To decide whether the point P is inside boundary b1 -> b2.
// Can be used for any boundary defined by starting point b1 and ending point b2.
// Counter-clockwise order for clippin window.
//
// @param p  point P
// @param b1 starting point b1 of the boundary
// @param b2 starting point b2 of the boundary
//
// @return boolean var

bool inside(const Vertex &p, const Vertex &b1, const Vertex &b2)
{
	// left or right boundary
	if (b1.x == b2.x)
	{
		// left
		if (b1.y > b2.y)
		{
			if (p.x >= b1.x) return true;
			else return false;
		}
		// right
		else
		{
			if (p.x <= b1.x) return true;
			else return false;
		}
	}
	// upper or bottom boundary
	else
	{
		// bottom
		if (b1.x < b2.x)
		{
			if (p.y >= b1.y) return true;
			else return false;
		}
		// upper
		else
		{
			if (p.y <= b1.y) return true;
			else return false;
		}
	}
}

//
// intersect
//
// polygon edge p -> s intersects with boundary b1 -> b2, modifying intersecting point i
//
// Two intersecting cases: 1. horizontal boundary; 2. parallel boundary.
//
// @param p one point end of the polygon edge
// @param s one point end of the polygon edge
// @param b1 starting point b1 of the boundary
// @param b2 starting point b2 of the boundary
// @param i intersecting point i

void intersect(Vertex p, Vertex s, Vertex b1, Vertex b2, Vertex &i)
{

	// if boundary is parallel to y-axis
	if (b1.x == b2.x)
	{
		float dy = 1.0 * (s.y - p.y)/(s.x - p.x);
		i.x = b1.x;
		i.y = dy * (i.x - s.x) + s.y;
	}

	// boundary is parallel to x-axis
	else
	{
		float dy = 1.0 * (s.y - p.y)/(s.x - p.x);
		i.y = b1.y;
		i.x = (i.y - s.y)/dy + s.x;
	}
}

//
// clipping
//
// This works for single-boundary clipping to the polygon
// traverse the polygon vertices to update its stats,
// based on four cases intersecting with side of region.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list (array)
// @param outV  the outgoing vertex list
// @param out   the number of vertices in the clipped polygon 
// @param b1 starting point b1 of the boundary
// @param b2 starting point b2 of the boundary

void SHPC(const Vertex inV[], list<Vertex> &outV, const int &in, int &out, const Vertex &b1, const Vertex &b2)
{
	// initialization
	int out_length {0};
	list<Vertex> outL;
	Vertex p {inV[in - 1]};

	for(int i {0}; i < in; ++i)
	{
		Vertex s {inV[i]};
		if (inside(s, b1, b2)) // case 1 & 4
		{
			if (inside(p, b1, b2)) // case 1
			{
				outL.push_back(s);
				++out_length;
			}
			else                   // case 4
			{
				Vertex i {0, 0};
				intersect(p, s, b1, b2, i);
				outL.push_back(i);
				outL.push_back(s);
				out_length += 2;
			}
		}
		else // cse 2 & 3
		{
			if (inside(p, b1, b2)) // case 2
			{
				Vertex i {0, 0};
				intersect(p, s, b1, b2, i);
				outL.push_back(i);
				++out_length;
			}
		}
		p = s;
	}

	outV = outL;
	out = out_length;
}

///
// clipPolygon
//
// Clip the polygon with vertex count in and vertices inV against the
// rectangular clipping region specified by lower-left corner ll and
// upper-right corner ur. The resulting vertices are placed in outV.
//
// The routine should return the with the vertex count of polygon
// resulting from the clipping.
//
// @param in    the number of vertices in the polygon to be clipped
// @param inV   the incoming vertex list
// @param outV  the outgoing vertex list
// @param ll    the lower-left corner of the clipping rectangle
// @param ur    the upper-right corner of the clipping rectangle
//
// @return number of vertices in the polygon resulting after clipping
///
int clipPolygon( int in, const Vertex inV[], Vertex outV[],
		 Vertex ll, Vertex ur )
{
    // YOUR CODE GOES HERE
	list<Vertex> out1 {list<Vertex>()};
	list<Vertex> out2 {list<Vertex>()};
	list<Vertex> out3 {list<Vertex>()};
	list<Vertex> out4 {list<Vertex>()};
	int out1_length {0};
	int out2_length {0};
	int out3_length {0};
	int out4_length {0};		
	Vertex lr {ur.x, ll.y};
	Vertex ul {ll.x, ur.y};

	// clipping with lower boundary
	SHPC(inV, out1, in, out1_length, ll, lr);
	Vertex out1_arr[out1_length];
	int count{0};
	for (auto i {out1.begin()}; i != out1.end(); ++i)
	{
		out1_arr[count] = *i;
		++count;
	}

	// clipping with right boundary
	SHPC(out1_arr, out2, out1_length, out2_length, lr, ur);
	Vertex out2_arr[out2_length];
	count = 0;;
	for (auto i {out2.begin()}; i != out2.end(); ++i)
	{
		out2_arr[count] = *i;
		++count;
	}

	// clipping with upper boundary
	SHPC(out2_arr, out3, out2_length, out3_length, ur, ul);
	Vertex out3_arr[out3_length];
	count = 0;;
	for (auto i {out3.begin()}; i != out3.end(); ++i)
	{
		out3_arr[count] = *i;
		++count;
	}

	/// clipping with left boundary
	SHPC(out3_arr, out4, out3_length, out4_length, ul, ll);
	Vertex out4_arr[out4_length];
	count = 0;;
	for (auto i {out4.begin()}; i != out4.end(); ++i)
	{
		out4_arr[count] = *i;
		++count;
	}

	// deep copy the vertices list to outV array
	for (int i{0}; i < out4_length; ++i)
	{
		outV[i] = out4_arr[i];
	}

	return out4_length; // return the number of vertices from clipped polygon
}
