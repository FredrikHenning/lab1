#version 440

out vec3 frag_colour;

vec3 sum = vec3(0.0);

in vec3 normal; vec3 normNormal = vec3(normalize(normal));
in vec3 viewPos;
in vec3 pos;

uniform int light_count;
uniform vec3 light_position[4]; // can have up to 4 light sources
uniform vec3 light_colour[4];

vec3 diff()
{ 
  vec3 pL = vec3(0.95f, 0.64f, 0.54f);
  float pi = 3.14f;

  vec3 lam = vec3(pL/pi);
  
  return lam;
}

vec3 freshnel_equation(vec3 h, vec3 l)
{
    vec3 F0 = vec3(0.95f, 0.64f, 0.54f);
    vec3 F = F0 + (1.0 - F0)*pow(1 - max(dot(h,l), 0.0), 5);
    return F;
}

float beckmann_dist(vec3 m, vec3 n)
{
    int x = 0;
    if(dot(n,m) > 0) x = 1;
    float PI = 3.1415;
    float a = 0.8f;
    float D = ( x/(PI*pow(a,2)*pow(dot(n,m), 4))) * exp((pow(dot(n,m), 2) - 1) / (pow(a,2) * pow(dot(n,m), 2)) );
    return D;
}

float geometric(vec3 h, vec3 v, vec3 l, vec3 n)
{
    float G = min( 1, (2*dot(n,h)*dot(n,v)) / dot(v,h) );
    float G2 = min(G, (2*dot(n,h)*dot(n,l)) / dot(v,h));
    return G;
}

vec3 specular(vec3 l, vec3 v, vec3 n)
{
    vec3 h = normalize(l + v);
    vec3 F = freshnel_equation(h, l);
    float G = geometric(h, v, l, n);
    float D = beckmann_dist(h, n);
    vec3 f_spec = (F*G*D) / (4*abs(dot(n,l))*abs(dot(n,v)));
    return f_spec;
}

vec3 brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
{
    float kL = 0.5f;
    float kG = 0.5f;

    vec3 bp = vec3( kL * diff() + kG * specular(in_direction, out_direction, normal) );

    return bp;
}

void main () {

  for (int l = 0; l < light_count; ++l )
  {
    //Normalizing some vectors
    vec3 normLightColour = vec3(normalize(light_colour[l]));
    vec3 normLightPos = vec3(normalize(light_position[l])); 
    vec3 lDirection = vec3(normalize(light_position[l] - viewPos));

    //Sum of: incoming light colour*brdf*cosine
    sum = sum + vec3(normLightColour *
                    brdf(lDirection, normalize(-viewPos), normNormal ) *
                    max(dot(lDirection, normNormal), 0.0f));
  }

  //frag_colour = (normal.x + 2) * sum*3;
  frag_colour = sum*5;
}
