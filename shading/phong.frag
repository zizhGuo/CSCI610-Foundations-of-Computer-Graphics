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
in vec4 N;
in vec4 Lpos;
in vec4 vpos;

// or from the vertex shader) here
uniform vec4 OA; // ambient color
uniform float KA; // ambient reflection coefficient
uniform vec4 OD; // diffuse color
uniform float KD; // diffuse reflection coefficient
uniform vec4 OS; // specular color
uniform float SE; // specular exponent
uniform float KS; // specular reflection coefficient

uniform vec4 IS; // light color
uniform vec4 IA; // ambient light color


// Outgoing fragment color
out vec4 fragmentColor;

void main()
{
    // Add all illumination and shading code you need here

    vec4 normals = normalize(N); 
    vec4 L = normalize(Lpos - vpos); // calculate light vector
    vec4 R = reflect(L, normals); // calculate the reflective vector
    vec4 V = normalize(vpos);

    fragmentColor = IA * KA * OA + IS * KD * OD * max(dot(L.xyz, normals.xyz), 0) + OS *IS * KS * pow(max(dot(R.xyz, V.xyz), 0), SE); // apply phong model equation to get final color

    // by default, just use 80% gray for everything
    //fragmentColor = vec4( 0.8, 0.8, 0.8, 1.0 );
}
