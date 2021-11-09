#version 400
out vec4 frag_colour;
in vec4 pos;
void main () {

	// Set color proportional to the z position here, instead of a constant. 
	frag_colour = vec4((pos.z-0.5)*-1);
}
