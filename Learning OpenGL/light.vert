#version 460 core

layout (location = 0) in vec3 objectPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	float x = objectPosition.x;
	float y = objectPosition.y;
	gl_Position = camMatrix * model * vec4(x, y, objectPosition.z, 1.0);
}