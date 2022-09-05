#include "Texture.h"

Texture::Texture(std::string name, IMGTYPE imgType, GLenum interpolation, GLenum wrap)
{
	Texture::imgType = imgType;
	int width, height, numColCh;
	unsigned char* bytes;
	//stbi_set_flip_vertically_on_load(true);
	bytes = stbi_load((char *)name.c_str(), &width, &height, &numColCh, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &textureID);
	Bind(0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	if (numColCh == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	else if (numColCh == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	else if (numColCh == 1) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
	else throw std::invalid_argument("Invalid texture");

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	Unbind();
	Bind(0);
}

void Texture::Bind(GLuint unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Activate(GLuint unit, Shader& shader, const char* uniform)
{
	Texture::Bind(unit);
	glUniform1i(glGetUniformLocation(shader.ID, uniform), unit);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &textureID);
}