#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPosition;

out vec3 V;
out vec3 L;
out vec3 N;

void main()
{
	vec4 v = vec4(vertex.xyz,1.0);
	gl_Position = modelViewProjectionMatrix * v;

	vec3 vVertex = vec4( modelViewMatrix * v ).xyz;
	vec3 vLightpos = vec4( viewMatrix * vec4(lightPosition,1.0) ).xyz;
	
	V = -vVertex;
	L = vLightpos - vVertex;
	N = vec4( normalMatrix * vec4(normal.xyz,1.0) ).xyz;
}