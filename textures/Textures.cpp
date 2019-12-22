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
//  Contributor:  Zizhun Guo
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

// These variables are used in both Phong shading and texture mapping
GLfloat cyl_specExp  = 20.0f;
GLfloat quad_specExp = 50.0f;
GLfloat sph_specExp  = 80.0f;
GLfloat k[3]         = { 0.5f, 0.7f, 1.0f };

// These variables are used in the framework only when doing Phong
// shading; they will be unused once you have implemented texture mapping.
GLfloat cyl_ambient[4]      = { 0.50f, 0.10f, 0.90f, 1.00f };
GLfloat cyl_diffuse[4]      = { 0.89f, 0.00f, 0.00f, 1.00f };
GLfloat quad_ambdiffuse[4]  = { 0.10f, 0.85f, 0.20f, 1.00f };
GLfloat sph_ambient[4]      = { 0.50f, 0.50f, 0.50f, 1.00f };
GLfloat sph_diffuse[4]      = { 0.49f, 0.49f, 0.80f, 1.00f };
GLfloat specular[4]         = { 1.00f, 1.00f, 1.00f, 1.00f };

// Are we doing texture mapping instead of material properties?
bool usingTextures = false;

// Add any global definitions and/or variables you need here.

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
    // Add your code here.
    //
    //
    // For "Quad", using SOIL_load_OGL_texture() to directly generate texutres.
    //
    GLuint tex_happy = SOIL_load_OGL_texture(
        "happy.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | // set mipmaps; set inversion on Y coordinates
            SOIL_FLAG_TEXTURE_REPEATS); // set wrapping mode to repeat

    GLuint tex_angry = SOIL_load_OGL_texture(
        "angry.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
            SOIL_FLAG_TEXTURE_REPEATS);

    glActiveTexture(GL_TEXTURE0);  // active the texutre unit 0
    glBindTexture(GL_TEXTURE_2D, tex_happy);  // bind the happy face texture to current unit

    glActiveTexture(GL_TEXTURE1);   // active the texutre unit 1
    glBindTexture(GL_TEXTURE_2D, tex_angry);    // bind the angry face texture to current unit

    //
    // For "Sphere" and "Cylinder", using more complicated way by loading the image, sets the data,
    // sets the parameters independently, and loading the data to texture.
    //
    int width, height;
    unsigned char *image;
    GLuint tex_jupiter, tex_wall, tex_disc;
    glGenTextures(1, &tex_jupiter); // generate texture for sphere
    glGenTextures(1, &tex_wall); // generate texture for cylinder side
    glGenTextures(1, &tex_disc); // generate texture for cylinder disc

    // "Sphere"
    glActiveTexture(GL_TEXTURE2); // activate texture unit 2
    glBindTexture(GL_TEXTURE_2D, tex_jupiter); // bind the jupiter texture to current unit
    image = SOIL_load_image("jupiter.jpg", &width, &height, 0, SOIL_LOAD_RGB ); // loading the image
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




    // Cynlinder side
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, tex_wall);
    image = SOIL_load_image("wall.png", &width, &height, 0, SOIL_LOAD_RGB );
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

    // Cylinder disc
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, tex_disc);
    image = SOIL_load_image("disc.png", &width, &height, 0, SOIL_LOAD_RGB );
    if( image == 0 ) {     
        printf( "SOIL loading error: '%s'\n",             
            SOIL_last_result() ); 
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);



    
    // Be sure to set the global variable 'usingTextures' to "true"
    // once you have implemented your texture mapping code here,
    // in setTextures(), and in your shaders.
    //
    usingTextures = true;
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
    case OBJ_QUAD:
        glUniform1f( loc, quad_specExp );
        break;
    case OBJ_CYLINDER:  // FALL THROUGH
    case OBJ_DISCS:
        glUniform1f( loc, cyl_specExp );
        break;
    case OBJ_SPHERE:
        glUniform1f( loc, sph_specExp );
        break;
    }

    // Send down the reflective coefficients
    loc = getUniformLoc( program, "kCoeff" );
    glUniform3fv( loc, 1, k );

    // Send down the "are we texture mapping?" flag
    loc = getUniformLoc( program, "usingTextures" );
    glUniform1i( loc, usingTextures );

    ///////////////////////////////////////////////////////////
    // CODE DIFFERING BETWEEN PHONG SHADING AND TEXTURE MAPPING
    ///////////////////////////////////////////////////////////

    //
    // Check to see whether or not we are using texture mapping
    // on the object being drawn at the moment.
    //

    if( !usingTextures ) {

        ///////////////////////////////////////////////////////////
        // THIS CODE IS USED ONLY IF TEXTURE MAPPING IS ***NOT***
	// BEING DONE ON THIS OBJECT
        ///////////////////////////////////////////////////////////

        // specular color is identical for the objects
        loc = getUniformLoc( program, "specularColor" );
        glUniform4fv( loc, 1, specular );

        // ambient and diffuse vary from one object to another
        GLint dloc = getUniformLoc( program, "diffuseColor" );
        GLint aloc = getUniformLoc( program, "ambientColor" );

        switch( obj ) {
        case OBJ_QUAD:
            glUniform4fv( aloc, 1, quad_ambdiffuse );
            glUniform4fv( dloc, 1, quad_ambdiffuse );
            break;
        case OBJ_CYLINDER: // FALL THROUGH
        case OBJ_DISCS:
            glUniform4fv( aloc, 1, cyl_ambient );
            glUniform4fv( dloc, 1, cyl_diffuse );
            break;
        case OBJ_SPHERE:
            glUniform4fv( aloc, 1, sph_ambient );
            glUniform4fv( dloc, 1, sph_diffuse );
            break;
        }

	return;
    }

    ///////////////////////////////////////////////////////////
    // CODE FROM THIS POINT ON WILL BE USED ONLY IF TEXTURE
    // MAPPING IS BEING DONE ON THIS OBJECT
    ///////////////////////////////////////////////////////////

    //
    // Add your code here to implement texture mapping.
    //
        switch( obj ) {
        case OBJ_QUAD:
            glUniform1i(glGetUniformLocation(program, "texfront"), 0); // quad front face using texture unit 0
            glUniform1i(glGetUniformLocation(program, "texback"), 1); // back face using texture unit 1
            break;
        case OBJ_CYLINDER:
            glUniform1i(glGetUniformLocation(program, "texfront"), 3); // using texture unit 3
            break;
        case OBJ_DISCS:
            glUniform1i(glGetUniformLocation(program, "texfront"), 4); // using texture unit 4
            break;
        case OBJ_SPHERE:
            glUniform1i(glGetUniformLocation(program, "texfront"), 2); // using texture unit 2
            break;
        }
}
