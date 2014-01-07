#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

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

	void set_uniform(const char *uniform_name, GLint value);
	void set_uniform(const char *uniform_name, const glm::mat4 &value);

private:
	program(const program &);
	program &operator=(const program &);

	const GLuint name;
};
