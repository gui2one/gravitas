#include "camera.h"
namespace Orbiter {


Camera::Camera():m_fov_angle(45.0f), m_near(0.01f), m_far(100.0f), position(glm::vec3(1.0f, 0.0f, 0.0f)), target_position(glm::vec3(0.0f, 0.0f, 0.0f)), up_vector(glm::vec3(0.0f, 0.0f, 1.0f))
{
	
	
	
	
}

Camera::Camera(float ratio, float _fov_angle):m_fov_angle(45.0f), m_near(0.01f), m_far(100.0f), position(glm::vec3(1.0f, 0.0f, 0.0f)), target_position(glm::vec3(0.0f, 0.0f, 0.0f)), up_vector(glm::vec3(0.0f, 0.0f, 1.0f))
{

	projection = glm::mat4(1.0f) * glm::perspective(_fov_angle, ratio, 0.01f, 100.0f);
}


void Camera::setProjection(glm::mat4 _proj)
{
	projection = _proj;
}

void Camera::orbit(float u, float v, float dist)
{
	
	position = Orbiter::Utils::polar_to_euclidian(u, v, dist);
}
}// end namespace

