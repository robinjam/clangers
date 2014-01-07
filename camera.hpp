#pragma once

#include <glm/glm.hpp>

class camera
{
public:
	camera();

	void accelerate(float amount);
	void stop();
	void change_altitude(float amount);
	void change_angle(float amount);
	void update(double dt);

	glm::mat4 view_matrix();

private:
	float yaw, velocity;
	glm::vec3 position;
};
