#pragma once

#include <GL/glew.h>

class shader;

class program
{
public:
	program();
	~program();

	void attach_shader(const shader &shader);
	void detach_shader(const shader &shader);
	void link();

	void use();

private:
	program(const program &);
	program &operator=(const program &);

	const GLuint name;
};
