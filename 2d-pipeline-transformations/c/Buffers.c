///
//  Buffers.c
//
//  Vertex and element buffer implementations.
//
//  Author:  Warren R. Carithers
//  Date:    2017/02/11 22:34:54
//  Updated: 2019/01/25 18:37:18
//
//  This file should not be modified by students.
///

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "Canvas.h"

///
// initBuffer(buf) - reset the supplied BufferSet to its "empty" state
//
// @param buf - which buffer to reset
///
void initBuffer( BufferSet *buf ) {
    buf->vbuffer = buf->ebuffer = 0;
    buf->numElements = 0;
    buf->vSize = buf->eSize = buf->tSize = buf->cSize = buf->nSize = 0;
    buf->bufferInit = false;
}

///
// dumpBuffer(buf) - dump the contents of a BufferSet
//
// @param which - description of the buffer
// @param buf - the buffer to dump
///
void dumpBuffer( const char *which, BufferSet *buf ) {
    printf( "Dumping buffer %s (%s):\n", which,
        buf->bufferInit ? "initialized" : "not initialized" );
    printf( "  IDs: v %u e %u  #elements: %d\n", buf->vbuffer,
        buf->ebuffer, buf->numElements );
    printf( "  Sizes:  v %ld e %ld t %ld c %ld n %ld\n", buf->vSize,
        buf->eSize, buf->tSize, buf->cSize, buf->nSize );
}

///
// makeBuffer() - make a vertex or element array buffer
//
// @param target - which type of buffer to create
// @param data   - source of data for buffer (or NULL)
// @param size   - desired length of buffer
//
// @return the ID of the new buffer
//
GLuint makeBuffer( GLenum target, const void *data, GLsizei size ) {
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

///
// createBuffers(buf,canvas) create a set of buffers for the object
//     currently held in 'canvas'.
//
// @param B   - the BufferSet to be modified
// @param C   - the Canvas we'll use for drawing
///
void createBuffers( BufferSet *B, Canvas *C ) {

    // reset this BufferSet if it has already been used
    if( B->bufferInit ) {
        // must delete the existing buffer IDs first
        glDeleteBuffers( 1, &(B->vbuffer) );
        glDeleteBuffers( 1, &(B->ebuffer) );
        // clear everything out
        initBuffer( B );
    }

    ///
    // vertex buffer structure
    //
    // our vertex buffers will always have locations, but the
    // other fields may or may not be present; this depends on
    // how the shape was created
    //
    //             data        components   offset to beginning
    //          [ locations ]  XYZW         0
    //          [ colors    ]  RGBA         vSize
    //          [ normals   ]  XYZ          vSize+cSize
    //          [ t. coords ]  UV           vSize+cSize+nSize
    ///

    // get the vertex count
    B->numElements = canvasNumVertices( C );

    // if there are no vertices, there's nothing for us to do
    if( B->numElements < 1 ) {
        return;
    }

    // OK, we have vertices!
    float *points = canvasGetVertices( C );
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->vSize = B->numElements * 4 * sizeof(float);

    // accumulate the total vertex buffer sizee
    GLsizeiptr vbufSize = B->vSize;

    // get the color data (if there is any)
    float *colors = canvasGetColors( C );
    if( colors != NULL ) {
        B->cSize = B->numElements * 4 * sizeof(float);
        vbufSize += B->cSize;
    }

    // get the normal data (if there is any)
    float *normals = canvasGetNormals( C );
    if( normals != NULL ) {
        B->nSize = B->numElements * 3 * sizeof(float);
        vbufSize += B->nSize;
    }

    // get the (u,v) data (if there is any)
    float *uv = canvasGetUV( C );
    if( uv != NULL ) {
        B->tSize = B->numElements * 2 * sizeof(float);
        vbufSize += B->tSize;
    }

    // get the element data
    GLuint *elements = canvasGetElements( C );
    // #bytes = number of elements * bytes/element
    B->eSize = B->numElements * sizeof(GLuint);

    // first, create the connectivity data
    B->ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B->eSize );

    // next, the vertex buffer, containing vertices and "extra" data
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vbufSize );

    // copy in the location data
    glBufferSubData( GL_ARRAY_BUFFER, 0, B->vSize, points );

    // offsets to subsequent sections are the sum of 
    // the preceding section sizes (in bytes)
    GLintptr offset = B->vSize;

    // add in the color data (if there is any)
    if( B->cSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, offset, B->cSize, colors );
        offset += B->cSize;
    }

    // add in the normal data (if there is any)
    if( B->nSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, offset, B->nSize, normals );
        offset += B->nSize;
    }

    // add in the (u,v) data (if there is any)
    if( B->tSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, offset, B->tSize, uv );
        offset += B->tSize;
    }

    // sanity check!
    if( offset != vbufSize ) {
        fprintf( stderr,
            "*** createBuffers: size mismatch, offset %ld vbufSize %ld\n",
            offset, vbufSize );
    }

    // NOTE:  'points', 'colors', and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;

}

///
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param B       - BufferSet to use
// @param vp      - name of the position attribute variable
// @param vc      - name of the color attribute variable (or NULL)
// @param vn      - name of the normal attribute variable (or NULL)
// @param vt      - name of the texture coord attribute variable (or NULL)
///
void selectBuffers( GLuint program, BufferSet *B, 
    const char *vp, const char * vc, const char *vn, const char *vt ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B->vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B->ebuffer );

    // set up the vertex attribute variables

    // we always have position data
    GLint vPosition = glGetAttribLocation( program , vp );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    // cumulative byte offset for subsequent data sections
    int offset = B->vSize;

    // do we also have color?
    if( vc != NULL ) {
        GLint vColor = glGetAttribLocation( program, vc );
        glEnableVertexAttribArray( vColor );
        glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B->cSize;
    }

    // how about a surface normal?
    if( vn != NULL ) {
        GLint vNormal = glGetAttribLocation( program, vn );
        glEnableVertexAttribArray( vNormal );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B->nSize;
    }

    // what about texture coordinates?
    if( vt != NULL ) {
        GLint vTexCoord = glGetAttribLocation( program, vt );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(offset) );
        offset += B->tSize;
    }
}
