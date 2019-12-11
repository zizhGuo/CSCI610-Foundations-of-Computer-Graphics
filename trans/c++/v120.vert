//
// Alternate vertex shader for the transformation assignment
//
// Created by Warren R. Carithers 2016/04/22.
//
// Contributor:  YOUR_NAME_HERE
//

#version 120

// attribute:  vertex position
attribute vec4 vPosition;

// add other global shader variables to hold the
// parameters your application code is providing

void main()
{
    // By default, no transformations are performed.
    gl_Position =  vPosition;
}
