// Build on your code from Lab 1 here
#version 400
out vec3 frag_colour;
in vec3 pos;
in vec3 normal;
void main () {
	//frag_colour =(pos+normal + 1);
	frag_colour.x =(pos.x+normal.x + 1);
	frag_colour.y =(pos.y+normal.y + 1) ;
	frag_colour.z =(pos.z+normal.z + 1);
	//frag_colour = vec3(pos.z + 0.5);
	//vec4((pos.z-1.0)*-1);
}

