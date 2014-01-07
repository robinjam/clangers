#include "program.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

program::program()
: name(glCreateProgram())
{
}

program::~program()
{
	glDeleteShader(name);
}

void program::attach_shader(const shader &shader)
{
	glAttachShader(name, shader.name);
}

void program::detach_shader(const shader &shader)
{
	glDetachShader(name, shader.name);
}

void program::link()
{
	glLinkProgram(name);

	GLint info_log_length;
	glGetProgramiv(name, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length) {
		std::vector<GLchar> info_log(info_log_length);
		glGetProgramInfoLog(name, info_log_length, NULL, info_log.data());
		std::clog << info_log.data() << std::endl;
	}

	GLint link_status;
	glGetProgramiv(name, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE) {
		throw std::runtime_error("Unable to link program");
	}
}

void program::use()
{
	glUseProgram(name);
}

void program::set_uniform(const char *uniform_name, GLint value)
{
	use();

	GLint location = glGetUniformLocation(name, uniform_name);
	if (location == -1)
		std::cerr << "Warning: No uniform named '" << uniform_name << "' found." << std::endl;
	else
		glUniform1i(location, value);
}

void program::set_uniform(const char *uniform_name, const glm::mat4 &value)
{
	use();

	GLint location = glGetUniformLocation(name, uniform_name);
	if (location == -1)
		std::cerr << "Warning: No uniform named '" << uniform_name << "' found." << std::endl;
	else
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
