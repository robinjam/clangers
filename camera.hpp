#pragma once

#include <glm/glm.hpp>

struct camera
{
	camera();

	void accelerate(float amount);
	void stop();
	void change_altitude(float amount);
	void change_angle(float amount);
	void update(double dt);
	void preset();

	glm::mat4 view_matrix();

	float yaw, velocity;
	glm::vec3 position;
};
