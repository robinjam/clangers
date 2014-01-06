#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

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

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);

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
