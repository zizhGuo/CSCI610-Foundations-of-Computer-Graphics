///
//  Textures
//
//  Simple class for setting up texture mapping parameters.
//
//  Created by Warren R. Carithers 2016/11/22.
//  Updates: 2019/11/23 by wrc.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2019 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Zizhun Guo 2019/12/12
//
//  This code can be compiled as either C or C++.
///

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#include <stdbool.h>
#endif

#include "Textures.h"
#include "Shapes.h"
#include "Utils.h"

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
#include <SOIL/SOIL.h>

#ifdef __cplusplus
using namespace std;
#endif

// Local variables using for texture shader
GLfloat cap_grey_specExp1  = 20.0f;
GLfloat cap_green_specExp1 = 50.0f;
GLfloat ground_specExp1  = 10.0f;

GLfloat k1[3]         = { 0.5f, 0.7f, 1.0f };

GLfloat lpDefault1[4]  = { 15.0f, 18.0f, 20.0f, 1.0f };
GLfloat lightpos1[4]   = { 15.0f, 18.0f, 20.0f, 1.0f };

// private lighting data
static GLfloat lightcolor1[4] = {  1.0f,  1.0f,  1.0f, 1.0f };
static GLfloat amblight1[4]   = {  0.7f,  0.7f,  0.7f, 1.0f };

GLfloat cap_grey_ambient1[4]      = { 0.50f, 0.10f, 0.90f, 1.00f };
GLfloat cap_grey_diffuse1[4]      = { 0.89f, 0.00f, 0.00f, 1.00f };
GLfloat cap_green_ambient1[4]      = { 0.50f, 0.10f, 0.90f, 1.00f };
GLfloat cap_green_diffuse1[4]      = { 0.89f, 0.00f, 0.00f, 1.00f };
GLfloat ground_ambient1[4]      = { 0.50f, 0.10f, 0.90f, 1.00f };
GLfloat ground_diffuse1[4]      = { 0.89f, 0.00f, 0.00f, 1.00f };
GLfloat specular1[4]         = { 1.00f, 1.00f, 1.00f, 1.00f };
///
// This function initializes all texture-related data structures for
// the program.  This is where texture buffers should be created, where
// texture images should be loaded, etc.
//
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
///
void initTextures( void )
{   

    //
    // Load the texture for silowall
    // 
    int width, height;
    unsigned char *image;
    GLuint tex_jupiter, tex_wall, tex_disc;
    glGenTextures(1, &tex_jupiter); // generate texture for sphere
    glGenTextures(1, &tex_wall); // generate texture for cylinder side
    glGenTextures(1, &tex_disc); // generate texture for cylinder disc

    // Load the texture for silowall
    glActiveTexture(GL_TEXTURE0); // activate texture unit 2
    glBindTexture(GL_TEXTURE_2D, tex_jupiter); // bind the jupiter texture to current unit
    image = SOIL_load_image("silowall.jpg", &width, &height, 0, SOIL_LOAD_RGB ); // loading the image
    if( image == 0 ) {     
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() ); 
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // sets S-axis wrapping mode as repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // sets T-axis wrapping mode as repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // sets filtering mode for magnification as nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // sets filtering mode for minification as linear
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // enable mipmap generation
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); // load the image to texture
    SOIL_free_image_data(image); // free space




    // Load the texture for barnbody
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_wall);
    image = SOIL_load_image("barnbody.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    if( image == 0 ) {     
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() ); 
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);


}

///
// This function sets up the parameters for texture use.
//
// You will need to modify this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param obj     The object type of the object being drawn
///
void setTextures( GLuint program, int obj )
{
    ///////////////////////////////////////////////////
    // CODE COMMON TO PHONG SHADING AND TEXTURE MAPPING
    //
    // DO NOT REMOVE THIS SECTION OF CODE
    ///////////////////////////////////////////////////

    // Set the specular exponent for the object
    GLint loc  = getUniformLoc( program, "specExp" );
    switch( obj ) {
    case CAP1:
    case CAP2:  // FALL THROUGH
    case CAP4:
        glUniform1f( loc, cap_grey_specExp1 );
        break;
    case CAP3:
        glUniform1f( loc, cap_green_specExp1 );
        break;
    case GROUND:
        glUniform1f( loc, ground_specExp1 );
        break;
    case SILO1:
    case SILO2:
    case BARN1:  // FALL THROUGH
    case BARN2:
        glUniform1f( loc, cap_grey_specExp1 );
        break;
    }

    // Send down the reflective coefficients
    loc = getUniformLoc( program, "kCoeff" );
    glUniform3fv( loc, 1, k1 );

    //
    // Add your code here to implement texture mapping.
    //
        switch( obj ) {
        case SILO1:
            glUniform1i(glGetUniformLocation(program, "tex"), 0); // using texture unit 3
            break;
        case SILO2:
            glUniform1i(glGetUniformLocation(program, "tex"), 0); // using texture unit 4
            break;
        case BARN1:
            glUniform1i(glGetUniformLocation(program, "tex"), 1); // using texture unit 2
            break;
        case BARN2:
            glUniform1i(glGetUniformLocation(program, "tex"), 1); // using texture unit 2
            break;
        }

    // Lighting parameters
    loc = getUniformLoc( program, "lightPosition" );
    glUniform4fv( loc, 1, lightpos1 );

    loc = getUniformLoc( program, "lightColor" );
    glUniform4fv( loc, 1, lightcolor1 );

    loc = getUniformLoc( program, "ambientLight" );
    glUniform4fv( loc, 1, amblight1 );
}
