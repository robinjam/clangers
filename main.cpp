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
#include "camera.hpp"

namespace
{
	camera cam;
	bool tour = false;

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

	void do_tour(double t)
	{
		if (t < 5) {
			glm::vec3 start = glm::vec3(0.f, 2.f, 0.5f);
			glm::vec3 step = (glm::vec3(-0.469959, 1.13729, 0.43129) - start) / glm::vec3(5);
			cam.position = start + step * glm::vec3(t);
			cam.yaw = 3.14f - 0.113472 * t;
		}
		else if (t > 10 && t < 15) {
			double u = t - 10;
			glm::vec3 start(-0.469959, 1.13729, 0.43129);
			glm::vec3 step = (glm::vec3(2.05243, 3.91877, 0.983068) - start) / glm::vec3(5);
			cam.position = start + step * glm::vec3(u);

			double yaw_start = 2.57264f;
			double yaw_end = 3.73302f;
			cam.yaw = yaw_start + ((yaw_end - yaw_start) / 5) * u;
		}
		else if (t > 20 && t < 25) {
			double u = t - 20;
			glm::vec3 start(2.05243, 3.91877, 0.983068);
			glm::vec3 step = (glm::vec3(-6.32559, -3.20083, 2.01724) - start) / glm::vec3(5);
			cam.position = start + step * glm::vec3(u);

			double yaw_start = 3.73302f;
			double yaw_end = 0.99759f;
			cam.yaw = yaw_start + ((yaw_end - yaw_start) / 5) * u;
		}
		else if (t > 30 && t < 35) {
			double u = t - 30;
			glm::vec3 start(-6.32559, -3.20083, 2.01724);
			glm::vec3 step = (glm::vec3(-1.20865, 0.0177682, 2.09559) - start) / glm::vec3(5);
			cam.position = start + step * glm::vec3(u);

			double yaw_start = 0.99759f;
			double yaw_end = -0.756993f;
			cam.yaw = yaw_start + ((yaw_end - yaw_start) / 5) * u;
		}
		else if (t > 40 && t < 45) {
			double u = t - 40;
			glm::vec3 start(-1.20865, 0.0177682, 2.09559);
			glm::vec3 step = (glm::vec3(0.f, 2.f, 0.5f) - start) / glm::vec3(5);
			cam.position = start + step * glm::vec3(u);

			double yaw_start = -0.756993f;
			double yaw_end = 3.14f;
			cam.yaw = yaw_start + ((yaw_end - yaw_start) / 5) * u;
		}
		else if (t > 45) {
			tour = false;
		}
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
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glClearColor(1.0, 1.0, 1.0, 0.0);

		shader transform(GL_VERTEX_SHADER), phong(GL_FRAGMENT_SHADER), fullscreen_triangle(GL_VERTEX_SHADER), skybox(GL_FRAGMENT_SHADER);
		transform.load("shaders/transform.glsl");
		phong.load("shaders/phong.glsl");
		fullscreen_triangle.load("shaders/fullscreen_triangle.glsl");
		skybox.load("shaders/skybox.glsl");

		program test;
		test.attach_shader(transform);
		test.attach_shader(phong);
		test.link();
		test.detach_shader(transform);
		test.detach_shader(phong);

		program test2;
		test2.attach_shader(fullscreen_triangle);
		test2.attach_shader(skybox);
		test2.link();
		test2.detach_shader(fullscreen_triangle);
		test2.detach_shader(skybox);

		glm::mat4 projection = glm::perspective(45.f, 1024.f / 576.f, 0.01f, 1000.f);

		test.set_uniform("projection", projection);
		test2.set_uniform("projection", projection);

		mesh clanger;
		clanger.load("meshes/clanger");

		mesh clanger2;
		clanger2.load("meshes/clanger2");

		mesh boat;
		boat.load("meshes/boat");

		mesh lid1;
		lid1.load("meshes/lid1");

		mesh lid2;
		lid2.load("meshes/lid2");

		mesh lid3;
		lid3.load("meshes/lid3");

		mesh moon_surface;
		moon_surface.load("meshes/moon_surface");

		GLuint tex[8];
		glGenTextures(8, tex);

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

		glBindTexture(GL_TEXTURE_2D, tex[3]);
		load_texture("textures/lid_diffuse.bmp", GL_RGB, GL_BGRA);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex[4]);
		load_texture("textures/lid_specular.bmp", GL_RGBA, GL_BGRA);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex[5]);
		load_texture("textures/moon_surface_diffuse.bmp", GL_RGB, GL_BGR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex[6]);
		load_texture("textures/boat_diffuse.bmp", GL_RGB, GL_BGR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex[7]);
		load_texture("textures/boat_specular.bmp", GL_RGBA, GL_BGRA);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		test.set_uniform("diffuse_map", 0);
		test.set_uniform("specular_map", 1);
		test.set_uniform("cubemap", 2);

		test2.set_uniform("cubemap", 0);

		GLuint vao;
		glGenVertexArrays(1, &vao);

		double t = glfwGetTime();

		while (!glfwWindowShouldClose(window)) {
			double dt = glfwGetTime() - t;

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			if (tour) {
				do_tour(t);
				if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
					tour = false;
				}
			}
			else {
				if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
					cam.change_angle(-1.f * float(dt));
				}
				if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
					cam.change_angle(1.f * float(dt));
				}
				if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
					cam.accelerate(1.f * float(dt));
				}
				if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
					cam.accelerate(-1.f * float(dt));
				}
				if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
					cam.stop();
				}
				if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
					cam.change_altitude(1.f * float(dt));
				}
				if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
					cam.change_altitude(-1.f * float(dt));
				}
				if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
					cam.preset();
				}
				if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
					tour = true;
					cam.stop();
					glfwSetTime(0);
				}
			}

			cam.update(dt);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = cam.view_matrix();
			t += dt;
			test.set_uniform("view", view);
			test2.set_uniform("view", view);
			test.set_uniform("model", model);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_CUBE_MAP, tex[2]);
			glBindVertexArray(vao);
			test2.use();
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			glClear(GL_DEPTH_BUFFER_BIT);

			glBindTexture(GL_TEXTURE_2D, tex[3]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex[4]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tex[2]);
			test.use();
			lid1.render();
			lid2.render();
			lid3.render();
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glBindTexture(GL_TEXTURE_2D, tex[5]);
			test.use();
			moon_surface.render();
			glBindTexture(GL_TEXTURE_2D, 0);

			model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (sin(glfwGetTime()) + 1) * 0.1f));
			test.set_uniform("model", model);
			glBindTexture(GL_TEXTURE_2D, tex[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tex[2]);
			test.use();
			clanger.render();
			model = glm::rotate(glm::mat4(1.f), float(glfwGetTime()) * -10.f, glm::vec3(0.f, 0.f, 1.f));
			test.set_uniform("model", model);
			clanger2.render();
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);

			glBindTexture(GL_TEXTURE_2D, tex[6]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex[7]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, tex[2]);
			test.use();
			boat.render();
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
