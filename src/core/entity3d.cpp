#include "entity3d.h"

namespace Orbiter
{
	Entity3d::Entity3d() :
		m_transforms(glm::mat4(1.0f)),
		m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_scale(glm::vec3(1.0f, 1.0f, 1.0f))
	{

	}

	void Entity3d::init()
	{
	}

	void Entity3d::update()
	{
	}

	void Entity3d::applyTransforms()
	{
		glm::mat4 temp = glm::mat4(1.0f);
		temp = glm::translate(temp, m_position);

		temp = glm::rotate(temp, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_transforms = temp;
	}
} // end namespace

