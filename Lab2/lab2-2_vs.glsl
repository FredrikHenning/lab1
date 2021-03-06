#version 400

layout(location=0) in vec4 vp;
layout(location=1) in vec4 normals;

out vec3 pos;
out vec3 normal;
out vec3 viewPos;

uniform mat4 modelViewMatrixpos;
uniform mat4 projectionModelViewMatrixpos;
void main () {
  
	gl_Position = projectionModelViewMatrixpos * vp;
	pos = gl_Position.xyz; //(modelViewMatrixpos * vp).xyz;
	normal = (modelViewMatrixpos * normals).xyz;
	viewPos = (modelViewMatrixpos * vp).xyz;
};
  