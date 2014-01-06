#include "mesh.hpp"

#include <stdexcept>
#include <fstream>
#include <string>
#include <iostream>

mesh::mesh()
{
	glGenVertexArrays(1, &vertex_array_name);
	glGenBuffers(2, buffer_names);
}

mesh::~mesh()
{
	glDeleteBuffers(2, buffer_names);
	glDeleteVertexArrays(1, &vertex_array_name);
}

void mesh::load(const char *filename)
{
	std::ifstream in(filename, std::ifstream::binary);
	if (!in.good()) {
		throw std::runtime_error(std::string("Unable to open ") + filename);
	}

	uint32_t magic_number;
	in.read((char *) &magic_number, sizeof(uint32_t));
	if (magic_number != 2811273200) {
		throw std::runtime_error("Invalid mesh format - magic number mismatch");
	}

	glBindVertexArray(vertex_array_name);

	uint32_t vbo_size, vbo_usage, ibo_size, ibo_usage, num_indices, num_attributes;
	in.read((char *) &vbo_size, sizeof(uint32_t));
	in.read((char *) &vbo_usage, sizeof(uint32_t));
	in.read((char *) &ibo_size, sizeof(uint32_t));
	in.read((char *) &ibo_usage, sizeof(uint32_t));
	in.read((char *) &num_indices, sizeof(uint32_t));
	in.read((char *) &num_attributes, sizeof(uint32_t));

	glBindBuffer(GL_ARRAY_BUFFER, buffer_names[0]);
	glBufferData(GL_ARRAY_BUFFER, vbo_size, NULL, vbo_usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_names[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo_size, NULL, ibo_usage);

	for (uint32_t i = 0; i < num_attributes; ++i) {
		uint32_t index; int32_t size; uint8_t normalized; uint32_t stride; uint32_t pointer;
		in.read((char *) &index, sizeof(uint32_t));
		in.read((char *) &size, sizeof(int32_t));
		in.read((char *) &normalized, sizeof(uint8_t));
		in.read((char *) &stride, sizeof(uint32_t));
		in.read((char *) &pointer, sizeof(uint32_t));
		glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride, reinterpret_cast<const GLvoid *>(pointer));
		glEnableVertexAttribArray(index);
	}

	void *vbo = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	in.read((char *) vbo, vbo_size);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	void *ibo = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	in.read((char *) ibo, ibo_size);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	this->num_indices = num_indices;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void mesh::render()
{
	glBindVertexArray(vertex_array_name);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}
