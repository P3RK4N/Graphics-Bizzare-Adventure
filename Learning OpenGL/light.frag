#version 460 core

out vec4 frag;

uniform vec4 lightColor;

void main()
{
	frag = lightColor;
}