#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "program.hpp"
#include "mesh.hpp"

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

		glfwWindowHint(GLFW_SAMPLES, 16);

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

		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		shader transform(GL_VERTEX_SHADER), solid_red(GL_FRAGMENT_SHADER);
		transform.load("shaders/transform.glsl");
		solid_red.load("shaders/solid_red.glsl");

		program test;
		test.attach_shader(transform);
		test.attach_shader(solid_red);
		test.link();
		test.detach_shader(transform);
		test.detach_shader(solid_red);

		test.use();

		glm::mat4 modelview = glm::lookAt(glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 projection = glm::perspective(45.f, 1024.f / 576.f, 1.f, 100.f);

		test.set_uniform("projection", projection);

		mesh clanger;
		clanger.load("meshes/clanger");

		while (!glfwWindowShouldClose(window)) {
			modelview = glm::rotate(modelview, float(glfwGetTime() * 45), glm::vec3(0.f, 1.f, 0.f));
			test.set_uniform("modelview", modelview);
			glfwSetTime(0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			clanger.render();

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
