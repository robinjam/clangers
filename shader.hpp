#pragma once

#include <GL/glew.h>

class shader
{
	friend class program;

public:
	shader(GLenum type);
	~shader();

	void compile(const GLchar *source);
	void load(const char *filename);

private:
	shader(const shader &);
	shader &operator=(const shader &);

	const GLuint name;
};
