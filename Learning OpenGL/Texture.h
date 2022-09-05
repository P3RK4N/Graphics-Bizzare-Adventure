#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<string>

#include<stb_image.h>
#include<glad/glad.h>

#include "shaderClass.h"

enum IMGTYPE
{
	DIFFUSE, SPECULAR
};
static const char* IMGTYPE_STR[] =
{
	"diffuse", "specular"
};

class Texture
{
	public:
		GLuint textureID;
		IMGTYPE imgType;

		Texture(std::string name, IMGTYPE imgType, GLenum interpolation, GLenum wrap);
		void Bind(GLuint unit);
		void Activate(GLuint unit, Shader& shader, const char* uniform);
		void Unbind();
		void Delete();
};
#endif
