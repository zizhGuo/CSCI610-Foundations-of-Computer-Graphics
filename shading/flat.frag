#version 130

// Use this as the basis for each of your fragment shaders.
// Besure to replace "SHADER" with the specific shader type
// and "YOUR_NAME_HERE" with your own name.

//
// Fragment shader for SHADER shading.
//
// @author  RIT CS Department
// @author  Zizhun Guo
//

// Add any incoming variables (from the application
// or from the vertex shader) here
in vec4 color;

// Outgoing fragment color
out vec4 fragmentColor;

void main()
{
    // Add all illumination and shading code you need here

    // by default, just use 80% gray for everything
    //fragmentColor = vec4( 0.8, 0.8, 0.8, 1.0 );
    fragmentColor = color;
}
