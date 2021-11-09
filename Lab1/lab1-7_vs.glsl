#version 400

layout(location=0) in vec4 vp;
out vec4 pos;
uniform mat4 position;
void main () {
  
	gl_Position = position * vp;
	pos = vp;
};
  