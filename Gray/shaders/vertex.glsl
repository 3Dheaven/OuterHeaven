#version 440 core

layout (location = 0) in vec3 position;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0f);

    TexCoords = vec2(position.xy + 1.0) * 0.5;
}