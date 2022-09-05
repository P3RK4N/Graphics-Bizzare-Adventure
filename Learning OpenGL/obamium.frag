#version 460 core

in vec2 texcoord;
in vec3 worldPos;
in vec3 normalcoord;

out vec4 frag;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightWorldPos;

float light(vec3 lightVector, vec3 normal)
{
	return max(0.0, dot(lightVector,normal)) + 0.2f;
}

void main()
{
	vec3 lightVector = normalize(- worldPos + lightWorldPos);
	frag = texture(tex0, texcoord) * lightColor;
}