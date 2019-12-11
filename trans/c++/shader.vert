//
// Vertex shader for the transformation assignment
//
// Created by Warren R. Carithers 2016/04/22.
//
// Contributor:  Zizhun Guo
//

#version 130

// attribute:  vertex position
in vec4 vPosition;

// add other global shader variables to hold the
// parameters your application code is providing


uniform vec3 scale;
uniform vec3 rotate;
uniform vec3 translate;

uniform vec3 eyepoint;
uniform vec3 lookat;
uniform vec3 up;

uniform mat4 projection;

void main()
{	
	
	// scale transformation setup
	mat4 scaleTrans = mat4(scale.x, 0.0, 0.0, 0.0,
							0.0, scale.y, 0.0, 0.0,
							0.0, 0.0, scale.z, 0.0,
							0.0, 0.0, 0.0, 1.0);

	// rotate transformation setup
	vec3 cos_vec = cos(radians(rotate));
	vec3 sin_vec = sin(radians(rotate));
	mat4 x_rotateTrans = mat4(1.0, 0.0, 0.0, 0.0,
								0.0, cos_vec.x, sin_vec.x, 0.0,
								0, - sin_vec.x, cos_vec.x, 0.0,
								0.0, 0.0, 0.0, 1.0);
	mat4 y_rotateTrans = mat4(cos_vec.y, 0.0, - sin_vec.y, 0.0,
								0.0, 1.0, 0.0, 0.0,
								sin_vec.y, 0.0, cos_vec.y, 0.0,
								0.0, 0.0, 0.0, 1.0);
	mat4 z_rotateTrans = mat4(cos_vec.z, sin_vec.z, 0.0, 0.0,
								- sin_vec.z, cos_vec.z, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0);
	mat4 rotateTrans = x_rotateTrans * y_rotateTrans * z_rotateTrans;

	// translate transformation setup
	mat4 translateTrans = mat4(1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								translate.x, translate.y, translate.z, 1.0);

	// camera transformation setup
	vec3 n = normalize(eyepoint - lookat);
	vec3 u = normalize(cross(normalize(up), n));
	vec3 v = normalize(cross(n, u));
	mat4 cameraTrans = mat4(u.x, v.x, n.x, 0.0,
							u.y, v.y, n.y, 0.0,
							u.z, v.z, n.z, 0.0,
							- dot(u, eyepoint), - dot(v, eyepoint), - dot(n, eyepoint), 1.0);

    // By default, no transformations are performed.
    // gl_Position = vPosition;

	gl_Position =  projection * cameraTrans * translateTrans * rotateTrans * scaleTrans * vPosition;
}
