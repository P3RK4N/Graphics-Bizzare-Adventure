#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aUV;

out vec3 color;
out vec2 uv;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	float x = aPos.x;
	float y = aPos.y;
	gl_Position = camMatrix * model * vec4(x, y, aPos.z, 1.0);
	color = aCol;
	uv = aUV;
}