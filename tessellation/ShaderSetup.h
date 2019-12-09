///
//  ShaderSetup.h
//
//  Set up a GLSL shader based on supplied source files.
///

#ifndef _SHADERSETUP_H_
#define _SHADERSETUP_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

///
// GLEW and GLFW header files also pull in the OpenGL definitions
///

#ifndef __APPLE__
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

///
// Error codes returned by ShaderSetup()
///

typedef enum sError {
    E_NO_ERROR, E_VS_LOAD, E_FS_LOAD, E_VS_COMPILE,
    E_FS_COMPILE, E_SHADER_LINK
} ShaderError;

///
// readTextFile(name)
//
// Read the text file given as 'name'.
//
// Returns the contents of the file in a dynamically-allocated
// string buffer, or NULL if an error occurs.
///
GLchar *readTextFile( const char *name );

///
// printShaderInfoLog(shader)
//
// Print the information log from a shader compilation attempt
///
void printShaderInfoLog( GLuint shader );

///
// printProgramInfoLog(shader)
//
// Print a program information log
//
// This is identical to printShaderInfoLog(), except that it uses
// glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
// versions.
///
void printProgramInfoLog( GLuint shader );

///
// errorString(code)
//
// Returns a const char* with a text version of the supplied error code.
///
const char *errorString( ShaderError code );

///
// shaderSetup(vertex,fragment,err)
//
// Set up a GLSL shader program.
//
// Requires the name of a vertex program and a fragment
// program.  Returns a handle to the created GLSL program
//
// Arguments:
//      vert - vertex shader program source file
//      frag - fragment shader program source file
//      err  - pointer to status variable
//
// On success:
//      Returns the GLSL shader program handle, and sets the 'err'
//      parameter to E_NO_ERROR.
//
// On failure:
//      Returns 0, and assigns an error code to 'err'.
///
GLuint shaderSetup( const char *vert, const char *frag, ShaderError *err );

#endif
