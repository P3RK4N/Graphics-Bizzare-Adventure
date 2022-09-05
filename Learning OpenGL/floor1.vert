#version 460 core

layout (location = 0) in vec3 objectPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 uv;

out vec2 texcoord;
out vec3 worldPos;
out vec3 normalcoord;

uniform mat4 camMatrix;
uniform mat4 model;

void main()	
{
	vec4 worldPos4 = model * vec4(objectPosition,1.0f);
	//Clip position
	gl_Position = camMatrix * worldPos4;
	worldPos = worldPos4.xyz;
	texcoord = uv;
	normalcoord = normal;
}