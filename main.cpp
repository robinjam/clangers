#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "shader.hpp"
#include "program.hpp"
#include "mesh.hpp"

namespace
{
	void load_texture(const char *filename, GLint internalFormat = GL_RGB, GLenum format = GL_BGR, GLenum target = GL_TEXTURE_2D)
	{
		std::ifstream in(filename);
		if (!in.good()) {
			throw std::runtime_error(std::string("Unable to open ") + filename);
		}

		char header[54];
		in.read(header, 54);
		if (!in.good() || header[0] != 'B' || header[1] != 'M') {
			throw std::runtime_error(std::string("Invalid bitmap format"));
		}

		int32_t file_size = *(int32_t *)&(header[0x02]);
		int32_t header_size = *(int32_t *)&(header[0x0A]);
		int32_t width = *(int32_t *)&(header[0x12]);
		int32_t height = abs(*(int32_t *)&(header[0x16]));
		int32_t offset = header_size - 54;
		int32_t image_size = file_size - header_size;

		std::cout << "file_size: " << file_size << std::endl;
		std::cout << "header_size: " << header_size << std::endl;
		std::cout << "width: " << width << std::endl;
		std::cout << "height: " << height << std::endl;
		std::cout << "offset: " << offset << std::endl;
		std::cout << "image_size: " << image_size << std::endl;

		std::vector<char> image_data(image_size);
		in.read(image_data.data(), image_size);

		glTexImage2D(target, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image_data.data());
	}
}

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

		glClearColor(1.0, 1.0, 1.0, 0.0);

		shader transform(GL_VERTEX_SHADER), phong(GL_FRAGMENT_SHADER);
		transform.load("shaders/transform.glsl");
		phong.load("shaders/phong.glsl");

		program test;
		test.attach_shader(transform);
		test.attach_shader(phong);
		test.link();
		test.detach_shader(transform);
		test.detach_shader(phong);

		test.use();

		glm::mat4 modelview = glm::lookAt(glm::vec3(0.f, 5.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 projection = glm::perspective(45.f, 1024.f / 576.f, 1.f, 100.f);

		test.set_uniform("projection", projection);

		mesh clanger;
		clanger.load("meshes/clanger");

		GLuint tex[3];
		glGenTextures(3, tex);

		glBindTexture(GL_TEXTURE_2D, tex[0]);
		load_texture("textures/clanger_diffuse.bmp");
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex[1]);
		load_texture("textures/clanger_specular.bmp", GL_RGBA, GL_BGRA);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, tex[2]);
		load_texture("textures/cubemap_right.bmp", GL_RGB, GL_BGRA, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
		load_texture("textures/cubemap_left.bmp", GL_RGB, GL_BGRA, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
		load_texture("textures/cubemap_top.bmp", GL_RGB, GL_BGRA, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
		load_texture("textures/cubemap_bottom.bmp", GL_RGB, GL_BGRA, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
		load_texture("textures/cubemap_front.bmp", GL_RGB, GL_BGRA, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
		load_texture("textures/cubemap_back.bmp", GL_RGB, GL_BGRA, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		test.set_uniform("diffuse_map", 0);
		test.set_uniform("specular_map", 1);
		test.set_uniform("cubemap", 2);

		while (!glfwWindowShouldClose(window)) {
			modelview = glm::rotate(modelview, float(glfwGetTime() * 45), glm::vec3(0.f, 0.f, 1.f));
			test.set_uniform("modelview", modelview);
			glfwSetTime(0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tex[2]);
			clanger.render();
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

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
