#version 400
out vec3 frag_colour;
in vec3 position;
void main () {
  frag_colour = position + vec3(0.0, 0.0, 0.5);
}
