#version 330 core

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec3 InColor;
layout (location = 2) in vec2 InUV;

uniform mat4 MVP;

out vec3 Color;
out vec2 UV;

void main()
{
    Color = InColor;
    UV = InUV;
	gl_Position = MVP * vec4(InPosition,1.0);
}
