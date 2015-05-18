#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 TexCoords;
out vec3 fragPos;
out vec3 N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 rotationMatrix1;
uniform mat4 rotationMatrix2;

#define M_PI 3.141593

uint seed;
void seeding(uint s)
{
  seed = s;
}

uint next()
{
  seed *= 3039177861u;
  //seed = 214013 * seed + 2531011;
  return seed;
  }
float uniform_0_1()
  {
   float f = float(next())/4294967295u;
  // float f = float(next())/(pow(2.0,32.0));
   return(f);
	
	 
  };

float uniform_mM(float min, float max)
{
	return min + (uniform_0_1() * (max - min));	
};

uint SeparateBy1(unsigned int x) {
    x &= 0x0000ffff;                  
    x = (x ^ (x <<  8)) & 0x00ff00ff; 
    x = (x ^ (x <<  4)) & 0x0f0f0f0f; 
    x = (x ^ (x <<  2)) & 0x33333333; 
    x = (x ^ (x <<  1)) & 0x55555555; 
    return x;
}

uint morton2D(uint x,uint y)
{
 return SeparateBy1(x) | (SeparateBy1(y) << 1);
}

uint morton(uint x, uint y)
{
  uint z = 0;
  uint one = 1;
  for (uint i = 0; i < (1 * 8); ++i) {
    z = z | ((x & (one << i)) << i) | ((y & (one << i)) << (i + one));
  }
  return z;
};

int mortonEncode(vec2 p) 
{
	// no bitwise in webgl, urgh...
	// somebody optimize this please :)
    	int c = 0;
    	for (int i=14; i>=0; i--) {
		float e = pow(2.0, float(i));
        	if (p.x/e >= 1.0) {
            		p.x -= e;
            		c += int(pow(2.0, 2.0*float(i)));
        	}
	    	if (p.y/e >= 1.0) {
            		p.y -= e;
            		c += int(pow(2.0, 1.0+2.0*float(i)));
		}
        }
    	return c;
}

flat out int choiceTex;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	choiceTex = gl_InstanceID % 4;

    TexCoords = texCoords;



	fragPos = vec4( view * model * vec4(position, 1.0f) ).xyz;
	
	N = vec4(transpose(inverse(view * model)) * vec4(normal, 1.0f)).xyz;
}