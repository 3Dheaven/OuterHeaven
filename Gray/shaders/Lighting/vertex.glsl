#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
//layout (location = 3) in vec3 tangent;
//layout (location = 4) in vec3 bitangent;

out vec2 TexCoords;
out vec3 fragPos;
out vec3 N;

layout (std430, binding=1) readonly buffer matrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 normalMatrix;
};

void main()
{
	mat4 mv = view * model;
	fragPos = vec3(mv * vec4(position, 1.0f));
    gl_Position = projection * vec4(fragPos, 1.0);

    TexCoords = texCoords;

	/*
	vec4 vNormal = view * model * vec4(normalize(normal), 1.0);
	vec4 vTangent = view * model * vec4(normalize(tangent), 1.0);
	vec4 vBitangent = view * model * vec4(normalize(bitangent), 1.0);

	mat3 TBN = transpose(mat3(
        vTangent.xyz,
        vBitangent.xyz,
        vNormal.xyz
    ));
	*/

	//LightDirection_tangentspace = TBN * LightDirection_cameraspace;
    //EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
	
	N = vec3(normalMatrix * vec4(normal, 1.0f));
}