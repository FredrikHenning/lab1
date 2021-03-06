#version 440

out vec3 frag_colour;
vec3 sum = vec3(0.0);
in vec3 normal; vec3 normNormal = vec3(normalize(normal));
in vec3 position;
in vec3 viewPos;
in vec3 pos;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

vec3 lambertian_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
  vec3 p = vec3(1.0f, 1.0f, 1.0f);
  float pi = 3.14f;
  vec3 lam = vec3(p/pi);
  return lam;

}

void main () {

  for (int l = 0; l < light_count; ++l )
  {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions:incoming light colour*brdf*cosine
    
    //Normalizing some vectors
    vec3 normLightColour = vec3(normalize(light_colour[l]));
    vec3 normLightPos = vec3(normalize(light_position[l])); 
    vec3 lDirection = vec3(normalize(light_position[l] - viewPos));

    //Sum of: incoming light colour*brdf*cosine
    sum = sum + vec3(normLightColour *
                    lambertian_brdf(lDirection, normalize(-viewPos), normNormal ) *
                    max(dot(lDirection, normNormal), 0.0f));
  }
  frag_colour =  sum*5;
  //eller: 
  //frag_colour = (normal.x + 1) * sum*5;
}
