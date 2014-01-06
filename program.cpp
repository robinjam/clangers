#include "program.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

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
