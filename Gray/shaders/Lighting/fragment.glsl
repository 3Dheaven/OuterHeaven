#version 440 core

#extension GL_ARB_shader_storage_buffer_object : require

struct Light
{
	vec3 position;
	vec3 ambient, diffuse, specular;
	vec3 attenuation;
	vec2 cutOff;
	vec3 direction;
};

layout (std430, binding=0) readonly buffer LightData
{
	ivec3 nbTypeLights;
	Light lights[];
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

in vec3 fragPos;
in vec2 TexCoords;
in vec3 N;

out vec4 color;

uniform Material material;
uniform mat4 view;

// Function prototypes
vec3 CalcPointLight(Light light, vec3 normal, vec3 V);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 V);
vec3 CalcDirLight(Light light, vec3 normal, vec3 V);

vec3 ambientColor(Light light);
vec3 diffuseColor(Light light, vec3 normal);
vec3 specularColor(Light light, vec3 normal, vec3 V);

float attenuation(Light light);
float intensity(Light light);

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv);
vec3 perturb_normal(vec3 N, vec3 V, vec2 texcoord);

void main()
{
	float alpha = texture2D(material.texture_diffuse, TexCoords).w;
	if(alpha < 0.1) discard;

    vec3 result = vec3(0.0f);

	vec3 V = normalize(-fragPos);
	
	vec3 normal = normalize(N);
	if(material.texDNS.y != 0)
		normal = perturb_normal(normal, V, TexCoords);
    
	// Point Lights
	for(int i = 0; i < nbTypeLights.x; i++)
        result += CalcPointLight(lights[i], normal, V);
	
	// Spot Lights
    for(int i = 0; i < nbTypeLights.y; i++)
		result += CalcSpotLight(lights[nbTypeLights.x+i], normal, V);

	// Directional Lights
	for(int i = 0; i < nbTypeLights.z; i++)
		result += CalcDirLight(lights[nbTypeLights.x+nbTypeLights.y+i], normal, V);

	color = vec4(result, 1.0f);
}

vec3 ambientColor(Light light)
{
	vec3 ambient;

	if(material.texDNS.x != 0)
		ambient = texture2D(material.texture_diffuse, TexCoords).xyz * material.ambient * light.ambient;
	else
		ambient = material.ambient * light.ambient;

	return ambient;
}

vec3 diffuseColor(Light light, vec3 normal, vec3 L)
{
	vec3 diffuse;

	if(material.texDNS.x != 0)
		diffuse = max(dot(L,normal),0) * texture2D(material.texture_diffuse, TexCoords).xyz * material.diffuse * light.diffuse;
	else
		diffuse = max(dot(L,normal),0) * material.diffuse * light.diffuse;

	return diffuse;
}

vec3 specularColor(Light light, vec3 normal, vec3 V, vec3 L)
{
	vec3 specular;

	vec3 R = reflect(-L,normal);

	if(material.texDNS.z != 0)
		specular = pow(max(dot(R,V),0),32) * texture2D(material.texture_specular, TexCoords).xyz * light.specular;
	else
		specular = pow(max(dot(R,V),0),32) * material.specular * light.specular;

	return specular;
}

float attenuation(Light light)
{
	vec3 vLightPosition = vec3(view * vec4(light.position, 1.0));
	float dist = length(vLightPosition - fragPos);
    return 1.0f / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
}

float intensity(Light light)
{
	vec3 vLightPosition = vec4(view * vec4(light.position, 1.0)).xyz;
	vec3 vLightDirection = vec4(view * vec4(light.position + light.direction, 1.0)).xyz;
	vLightDirection = vLightDirection - vLightPosition;

	vec3 lightDir = normalize(vLightPosition - fragPos);

	// Spotlight intensity
    float theta = dot(lightDir, normalize(-vLightDirection));
    float epsilon = light.cutOff.x - light.cutOff.y;
    return clamp((theta - light.cutOff.y) / epsilon, 0.0, 1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 V)
{
	vec3 a, d, s;

	vec3 vLightPosition = vec4(view * vec4(0.0, 1.0, 0.0, 1.0)).xyz;
	vec3 vLightpointedDirection = vec4(view * (vec4(0.0, 1.0, 0.0, 0.0) + vec4(light.direction, 1.0))).xyz;
	vec3 L = normalize(vLightPosition - vLightpointedDirection);

	a = ambientColor(light);
	d = diffuseColor(light, normal, L);
	s = specularColor(light, normal, V, L);

	return a + d + s;
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(Light light, vec3 normal, vec3 V)
{
	vec3 a, d, s;
	float att, intens;

	vec3 vLightPosition = vec3(view * vec4(light.position, 1.0));
	vec3 L = normalize(vLightPosition - fragPos);

	a = ambientColor(light);
	d = diffuseColor(light, normal, L);
	s = specularColor(light, normal, V, L);

    att = attenuation(light);

	intens = intensity(light);

	a *= att;
	d *= att * intens;
	s *= att * intens;

	return a + d + s;
}

// Calculates the color when using a point light.
vec3 CalcPointLight(Light light, vec3 normal, vec3 V)
{
	vec3 a, d, s;
	float att;

	vec3 vLightPosition = vec3(view * vec4(light.position, 1.0));
	vec3 L = normalize(vLightPosition - fragPos);

	a = ambientColor(light);
	d = diffuseColor(light, normal, L);
	s = specularColor(light, normal, V, L);

    att = attenuation(light);

	a *= att;
	d *= att;
	s *= att;

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