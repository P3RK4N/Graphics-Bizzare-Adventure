#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightWorldPos;
// Gets the position of the camera from the main function
uniform vec3 cameraWorldPos;

vec4 direcLight(vec3 crntPos, vec3 Normal, vec2 texCoord)
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.2f;
	vec3 viewDirection = normalize(cameraWorldPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

void main()
{
	vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f));
	vec3 normal = normalize(Normal);
	float ambient = 0.2f;
	float diffuse = max(dot(normal, lightDir), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(cameraWorldPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDir, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	FragColor = (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
//	FragColor = (diffuse + 0.2f) * texture(diffuse0, texCoord) + (texture(specular0, texCoord).r * specular) * lightColor;
//	FragColor = direcLight(crntPos, Normal, texCoord);
}