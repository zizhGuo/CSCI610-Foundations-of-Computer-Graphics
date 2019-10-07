///
//  Vertex.h
//
//  Representation of a "Vertex".
//
//  Author:  Warren R. Carithers
//  Date:    2016/10/07 12:34:54
///

#ifndef _VERTEX_H_
#define _VERTEX_H_

///
// Information pertaining to a vertex
//
// For 2D assignments, we ignore the 'z' and 'w' components.
///

typedef struct st_Vertex {
    float x;
    float y;
    float z;
    float w;
} Vertex;

#endif
