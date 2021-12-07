#version 440

out vec3 frag_colour;

vec3 sum = vec3(0.0);

in vec3 normal; vec3 normNormal = vec3(normalize(normal));
in vec3 viewPos;
in vec3 pos;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

vec3 lambertian_brdf()
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
  vec3 pL = vec3(0.8f, 0.5f, 0.3f);
  float pi = 3.14f;

  vec3 lam = vec3(pL/pi);
  
  return lam;
}
vec3 gloss( vec3 in_direction, vec3 out_direction, vec3 normal ){
    vec3 pG = vec3(1.0f, 1.0f, 1.0f);
    float pi = 3.14f;
    float f = 100.0f;

    vec3 h = normalize(in_direction + out_direction);
    float nDOTh = dot(normal,h);
    float expNH = pow(nDOTh, 100);

    vec3 gl = vec3 (pG * ((8 + f)/(8*pi)) * expNH);

    return gl;
}
vec3 blinn_phong_brdf( vec3 in_direction, vec3 out_direction, vec3 normal)
{
    float kL = 0.9f;
    float kG = 0.1f;

    vec3 bp = vec3( kL * lambertian_brdf() + kG * gloss(in_direction, out_direction, normal) );

    return bp;
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
                    blinn_phong_brdf(lDirection, normalize(-viewPos), normNormal ) *
                    max(dot(lDirection, normNormal), 0.0f));
  }

  frag_colour = (normal.x + 2) * sum*3;
}
