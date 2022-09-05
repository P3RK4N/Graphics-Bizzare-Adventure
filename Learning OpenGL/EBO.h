#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<vector>

#include<glad/glad.h>

class EBO
{
	public:
		GLuint ID;
		EBO(std::vector<GLuint>& indices);
		EBO(): ID(0) {};

		void Bind();
		void Unbind();
		void Delete();

};

#endif