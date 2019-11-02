//
//  Rasterizer.cpp
//
//  Simple module that provides rasterization algorithms
//
//  Created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/09, 2019/09/14 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Zizhun Guo
//                zg2808@cs.rit.edu
//

#include <list>
#include <algorithm>
#include <iostream>

#include "Rasterizer.h"
#include "Vertex.h"

using namespace std;


// Edge
// @member ymax the max y of that edge
// @member x the current x coordinate
// @member dx the 1/m 
class Edge
{
	public:
		int ymax;
		float x;
		float dx;
		Edge(int a1, float a2, float a3) : ymax(a1), x(a2), dx(a3){}
};

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
///
Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}

///
// Draw a filled polygon.
//
// Implementation should use the scan-line polygon fill algorithm
// discussed in class.
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are supplied in the 'v' array parameter, such
// that the ith vertex is in v[i].
//
// You are to add the implementation here using only calls to the
// setPixel() function.
//
// @param n - number of vertices
// @param v - array of vertices
///
void Rasterizer::fillPolygon( int n, const Vertex v[] )
{
    // YOUR IMPLEMENTATION HERE
	int WINDOWS_HEIGHT {600};

	size_t maxY = 0;

  // Simple optimization: Get the highest y coordinate among all edges
	for (size_t i{0}; i < n; ++i)
	{
  	if (v[i].y > maxY)
  	{
  	  maxY = v[i].y;
  	}
	}
  // Initialize the ET and AT
	list<Edge> ET[WINDOWS_HEIGHT];
	for (size_t i{0}; i < maxY; ++i)
	{
  	ET[i] = list<Edge>() ;
	} 
  list <Edge> AT {list<Edge>()};

    
  // Add edges to ET based on ymin
	for (size_t i {0}; i != n; ++i)
    {
    	int x1 = v[i].x;
    	int x2 = v[(i + 1) % n].x;
    	int y1 = v[i].y;
    	int y2 = v[(i + 1) % n].y;

      // Igonore horizontal line
    	if (y1 == y2) continue;

      // Info of current edge <(x1, y1), (x2, y2)>
    	int ymin = y1 > y2 ? y2 : y1;
    	int ymax = y1 > y2 ? y1 : y2;
    	float x = y1 > y2 ? x2 : x1;
    	float dx = (x1 - x2) * 1.0f / (y1 - y2);

      // Initialize current edge and ET
    	Edge edge = Edge(ymax, x, dx);
    	ET[ymin].push_back(edge);

    }

    // Scan-line drawing from y = 0 to y = 600
  	for (size_t i {0}; i != WINDOWS_HEIGHT; ++i)
  	{
  		//drop tables
  		for (auto j {AT.begin()}; j != AT.end();)
  		{
  			if (j -> ymax == i)
  			{
  				AT.erase(j++);
  			}
  			else
  			{
  				++j;
  			}
  		}

    // Update AT by copying edges from ET
  		for (auto j {ET[i].begin()}; j != ET[i].end(); ++j)
  		{
  			AT.push_back(*j);
  		}

  	//Sort edges in AT

      // Sort based on x
  		AT.sort([](const Edge& a, const Edge& b){return a.x < b.x;});

      // For each pair, if x equals, sorting based on dx
      for (auto j {AT.begin()}; j != AT.end(); advance(j, 2))
      {
        auto nxt = next(j, 1);
        if (j -> x == nxt -> x)
        {
          if (j -> dx > nxt -> dx)
          {
            iter_swap(j, nxt);
          }          
        }
      }

    // Fill polygon by pairs in each scan-line
      for (auto j {AT.begin()}; j != AT.end(); advance(j, 2))
      {
        auto nxt = next(j, 1);
        float x_left = j -> x;
        int x1 {0};

        // left intersected point, if x is on the edge, draw it. Or round it up.
        if (x_left - (int)x_left > 0) {
          x1 = (int)x_left + 1;
        }
        else
        {
          x1 = (int)x_left;
        }

        // right intersected point, round it down.
        int x2 = (int)(nxt -> x);

        // For each line segment by paired intersected points, set pixels.
        for (int k {x1}; k != x2; ++k)
        {
          Vertex v {(float)k, (float)i};
          C.setPixel(v);
        }
      }

    // Update x in AT by adding dx.
      for (auto j {AT.begin()}; j != AT.end(); advance(j, 1))
      {
        j -> x += j -> dx;
      }
  	}
}
