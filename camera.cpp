#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

camera::camera()
: position(0.f, 2.f, 0.5f), yaw(3.14f)
{
}

void camera::accelerate(float amount)
{
	velocity += amount;
	if (velocity < 0.f) {
		stop();
	}
}

void camera::stop()
{
	velocity = 0.f;
}

void camera::change_altitude(float amount)
{
	position.z += amount;
	if (position.z < 0.f) {
		position.z = 0.f;
	}
}

void camera::change_angle(float amount)
{
	yaw += amount;
}

void camera::update(double dt)
{
	glm::vec3 direction = glm::vec3(sin(yaw), cos(yaw), 0);
	position += direction * velocity * float(dt);
}

glm::mat4 camera::view_matrix()
{
	glm::vec3 direction(sin(yaw), cos(yaw), 0);
	return glm::lookAt(position, position + direction, glm::vec3(0.f, 0.f, 1.f));
}
