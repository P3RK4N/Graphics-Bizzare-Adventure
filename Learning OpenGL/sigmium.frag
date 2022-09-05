#version 460 core

in vec3 color;
in vec2 uv;

out vec4 frag;

uniform sampler2D tex0;
uniform vec4 lightColor;

void main()
{
	frag = texture(tex0, uv) * lightColor;
}