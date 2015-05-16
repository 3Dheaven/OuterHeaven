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

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

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

	fragPos = vec4( view * model * vec4(position, 1.0f) ).xyz;
	
	N = vec4(transpose(inverse(view * model)) * vec4(normal, 1.0f)).xyz;
}