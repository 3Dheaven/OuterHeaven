#version 440 core

#define NR_POINT_LIGHTS 2

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	sampler2D texture_diffuse;
	sampler2D texture_normal;
	sampler2D texture_specular;

	ivec3 texDNS;
};

in vec3 vVertex;
in vec3 vLightpos;
in vec2 TexCoords;

in vec3 N;

out vec4 color;

uniform vec3 viewPos;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

uniform mat4 view;

uniform float shininess;

// Function prototypes
vec3 CalcPointLight(int i);
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv);
vec3 perturb_normal(vec3 N, vec3 V, vec2 texcoord);

void main()
{
    vec3 result = vec3(0.0f);
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(i);

	color = vec4(result, 1.0f);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(int i)
{
	vec3 vLightPosition = vec3(view * vec4(pointLights[i].position, 1.0));

	vec3 Vn = normalize(-vVertex);
	vec3 Ln = normalize(vLightPosition - vVertex);
	vec3 Nn = normalize(N);

	if(material.texDNS.y != 0)
		Nn = perturb_normal(Nn, Vn, TexCoords);

	vec3 R = reflect(-Ln,Nn);

	vec3 a, d, s;

	if(material.texDNS.x != 0)
	{
		a = texture2D(material.texture_diffuse, TexCoords).xyz * material.ambient * pointLights[i].ambient;
		d = max(dot(Ln,Nn),0) * texture2D(material.texture_diffuse, TexCoords).xyz * material.diffuse * pointLights[i].diffuse;
	}
	else
	{
		a = material.diffuse * pointLights[i].ambient;
		d = max(dot(Ln,Nn),0) * material.diffuse * pointLights[i].diffuse;
	}

	if(material.texDNS.z != 0)
		s = pow(max(dot(R,Vn),0),32) * texture2D(material.texture_specular, TexCoords).xyz * material.specular * pointLights[i].specular;
	else
		s = pow(max(dot(R,Vn),0),32) * material.specular * pointLights[i].specular;

	float dist = length(vLightPosition - vVertex);
    float attenuation = 1.0f / (1.0f + 0.009 * dist + 0.0032 * (dist * dist));

	a *= attenuation;
	d *= attenuation;
	s *= attenuation;

	return a + d + s;
}

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );
 
    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
 
    // construct a scale-invariant frame 
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
    return mat3( T * invmax, B * invmax, N );
}

vec3 perturb_normal( vec3 N, vec3 V, vec2 texcoord )
{
    // assume N, the interpolated vertex normal and 
    // V, the view vector (vertex to eye)
   vec3 map = texture(material.texture_normal, texcoord ).xyz;
   map = map * 255./127. - 128./127.;
    mat3 TBN = cotangent_frame(N, -V, texcoord);
    return normalize(TBN * map);
}