#version 460 core

layout (location = 0) in vec3 objectPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 col;
layout (location = 3) in vec2 uv;

out vec2 texcoord;
out vec3 worldPos;
out vec3 normalcoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
	vec4 worldPos4 = model * translation * -rotation * scale * vec4(objectPosition,1.0f);
	gl_Position = camMatrix * worldPos4;
	worldPos = worldPos4.xyz;
	texcoord = mat2(0.0, -1.0, 1.0, 0) * uv;
	normalcoord = normal;
}