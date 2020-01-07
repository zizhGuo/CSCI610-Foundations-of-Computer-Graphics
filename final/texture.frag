#version 130

// Use this as the basis for each of your fragment shaders.
// Be sure to replace "SHADER" with the specific shader type
// and "YOUR_NAME_HERE" with your own name.

//
// Fragment shader for SHADER shading.
//
// @author  RIT CS Department
// @author  Zizhun Guo
//

// Light position
in vec3 lPos;

// Vertex position (in clip space)
in vec3 vPos;

// Vertex normal
in vec3 vNorm;

// Texture coordinates
in vec2 texCoord;

// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Material properties
uniform float specExp;
uniform vec3 kCoeff;

// ADD ANY INCOMING VARIABLES (FROM THE APPLICATION
// OR FROM THE VERTEX SHADER) HERE
uniform sampler2D tex;

// OUTGOING DATA

// The final fragment color
out vec4 fragmentColor;

void main()
{
    // calculate lighting vectors
    vec3 L = normalize( lPos - vPos );
    vec3 N = normalize( vNorm );
    vec3 R = normalize( reflect(-L, N) );
    vec3 V = normalize( -(vPos) );

    vec4 ambient  = vec4(0.0);  // ambient color component
    vec4 diffuse  = vec4(0.0);  // diffuse color component
    vec4 specular = vec4(0.0);  // specular color component
    float specDot;  // specular dot(R,V) ^ specExp value

    ambient  = ambientLight * texture(tex, texCoord) * max(dot(N,L),0.0);
    diffuse  = lightColor * texture(tex, texCoord);
    specDot  = pow( max(dot(R,V),0.0), specExp );
    specular = lightColor * texture(tex, texCoord) * specDot;

    // calculate the final color
    vec4 color = (kCoeff.x * ambient) +
                 (kCoeff.y * diffuse) +
                 (kCoeff.z * specular);

    fragmentColor = color;
}
