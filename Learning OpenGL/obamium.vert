#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 texcoord;
out vec3 worldPos;
out vec3 normalcoord;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	worldPos = vec3(model * vec4(pos,1.0f));
	gl_Position = camMatrix * model * vec4(pos,1);
	texcoord = uv;
	normalcoord = normalize(normal);
}