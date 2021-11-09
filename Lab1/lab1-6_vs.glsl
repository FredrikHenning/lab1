#version 400

layout(location=0) in vec4 vp;
out vec4 pos;
uniform mat4 position;
void main () {
	
	//-------------------------------------------------------------------------//
	// Instead of passing the `vp` variable unaffected as below, 
	// apply the model, view and projection transform to vertex
	// positions here. Forward the position to the fragment shader using
	// an appropriate `out` variable.
	// -------------------------------------------------------------------------//

	gl_Position = position * vp;
	pos = gl_Position;
};
  
