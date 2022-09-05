#version 460 core

in vec2 texcoord;
in vec3 worldPos;
in vec3 normalcoord;
in vec3 color;

out vec4 frag;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightWorldPos;
uniform vec3 cameraWorldPos;

vec4 lambert(vec3 normalVector, vec3 lightVector, float strength)
{
	float ambient = 0.2f;
	float diffuse = max(dot(normalVector, lightVector), 0.0f);
	return lightColor * (diffuse * strength + ambient);
}

float specular(vec3 viewVector, vec3 reflectionVector, float strength)
{
	float specularLight = 0.5f;
	float specAmount = pow(max(dot(viewVector, reflectionVector), 0.0f), 16);
	float specular = specAmount * specularLight;
	return specular * strength;
}

void main()
{
	vec3 normalVector = normalize(normalcoord);
	vec3 lightVector = lightWorldPos - worldPos;
	float dist = length(lightVector);
	lightVector = normalize(lightVector);
	vec3 viewVector = normalize(cameraWorldPos - worldPos);
	vec3 reflectionVector = reflect(-lightVector, normalVector);

	float strength = 1.0f / (3.0f * dist * dist + 0.7f * dist + 1.0f);

	frag = texture(diffuse0, texcoord) * lambert(normalVector, lightVector, strength) + specular(viewVector, reflectionVector, strength) * texture(specular0, texcoord).r;
}