#version 400
in vec3 vp;
out vec3 pos;
uniform vec2 position_offset;
void main () {
  
	// Change this so that you don't just set `gl_Position` to `vp`, but
	// also add an offset from a `uniform` variable:
	gl_Position = vec4(vp.x + position_offset.x, vp.y + position_offset.y, vp.z, 1.0);
	
//The color will follow with the triangle
	pos = vp;

//The color will be stationary
	//pos.x = gl_Position.x;
	//pos.y = gl_Position.y;

};
  
