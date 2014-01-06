#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "shader.hpp"
#include "program.hpp"

int main(int, const char **)
{
	try {
		if (!glfwInit()) {
			throw std::runtime_error("Unable to initialize GLFW");
		}

		// Call glfwTerminate() on scope exit
		struct scope_guard { ~scope_guard() { glfwTerminate(); } } guard;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		GLFWwindow *window = glfwCreateWindow(1024, 576, "Clangers", NULL, NULL);
		if (!window) {
			throw std::runtime_error("Unable to create OpenGL 3.3 Core context");
		}

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if (GLenum err = glewInit()) {
			std::ostringstream stream;
			stream << "Unable to initialize GLEW: " << glewGetErrorString(err);
			throw std::runtime_error(stream.str());
		}

		// Clear the OpenGL error flag - glewInit() generates GL_INVALID_ENUM
		glGetError();

		std::clog << "[INFO] GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
		std::clog << "[INFO] GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
		std::clog << "[INFO] GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
		std::clog << "[INFO] GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		shader passthrough(GL_VERTEX_SHADER), solid_red(GL_FRAGMENT_SHADER);
		passthrough.load("shaders/passthrough.glsl");
		solid_red.load("shaders/solid_red.glsl");

		program test;
		test.attach_shader(passthrough);
		test.attach_shader(solid_red);
		test.link();
		test.detach_shader(passthrough);
		test.detach_shader(solid_red);

		test.use();

		GLuint vao, vbo;
		GLbyte vertices[] = { -1, -1, 0, 1, -1, 0, 0, 1, 0 };

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (const std::exception &ex) {
		std::cerr << "[ERROR] Unhandled exception of type `" << typeid(ex).name() << "`: " << ex.what() << std::endl;
		throw;
	}
	catch (...) {
		throw;
	}

	return 0;
}
