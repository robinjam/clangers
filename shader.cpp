#include "shader.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>
#include <fstream>

shader::shader(GLenum type)
: name(glCreateShader(type))
{
}

shader::~shader()
{
	glDeleteShader(name);
}

void shader::compile(const GLchar *source)
{
	glShaderSource(name, 1, &source, NULL);
	glCompileShader(name);

	GLint info_log_length;
	glGetShaderiv(name, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length) {
		std::vector<GLchar> info_log(info_log_length);
		glGetShaderInfoLog(name, info_log_length, NULL, info_log.data());
		std::clog << info_log.data() << std::endl;
	}

	GLint compile_status;
	glGetShaderiv(name, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		throw std::runtime_error("Unable to compile shader");
	}
}

void shader::load(const char *filename)
{
	std::ifstream in(filename);
	if (!in.good()) {
		throw std::runtime_error(std::string("Unable to open ") + filename);
	}
	std::string source((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	compile(source.c_str());
}
