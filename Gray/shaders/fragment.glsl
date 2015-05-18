#version 440 core

layout(binding=0) uniform sampler2D colorTexBuffer;
layout(binding=1) uniform sampler2D depthTexBuffer;

in vec2 TexCoords;

out vec4 Color;

void main()
{
	vec3 color = texture2D(colorTexBuffer, TexCoords).rgb;
	float depth = texture2D(depthTexBuffer, TexCoords).x;
 
	Color = vec4(color, 1.0);
	//Color = vec4(depth, depth, depth, 1.0);
}