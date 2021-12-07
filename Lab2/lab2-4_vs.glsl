#version 400

layout(location=0) in vec3 vp;
layout(location=1) in vec3 normals;

out vec3 pos;
out vec3 normal;
out vec3 viewPos;

uniform mat4 modelViewMatrixpos;
uniform mat4 projectionModelViewMatrixpos;
void main () {
  
	gl_Position = projectionModelViewMatrixpos * vec4(vp, 1.0f);
	pos = gl_Position.xyz; //(modelViewMatrixpos * vp).xyz;
	normal = (modelViewMatrixpos * vec4(normals, 1.0f)).xyz;
	viewPos = (modelViewMatrixpos * vec4(vp, 1.0f)).xyz;
};
  