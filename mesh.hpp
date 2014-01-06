#pragma once

#include <GL/glew.h>

class mesh
{
public:
	mesh();
	~mesh();

	void load(const char *filename);
	void render();

private:
	mesh(const mesh &);
	mesh &operator=(const mesh &);

	GLuint vertex_array_name;
	GLuint buffer_names[2];
	GLuint num_indices;
};
