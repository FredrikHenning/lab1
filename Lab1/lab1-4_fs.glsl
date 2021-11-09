#version 400
out vec3 frag_colour;
in vec3 pos;
uniform float modifier;
void main () {

	// Change this to use input from a `uniform` variable instead of just a constant color:
	frag_colour = pos + vec3(0.5, 0.5, 0.5); 
}
