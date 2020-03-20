#pragma once
#include "../pch.h"
#include "3d_utils.h"
namespace Orbiter {


	class Camera
	{
	public:
		Camera();
		Camera(float ratio, float _fov_angle);

		void setProjection(glm::mat4 _proj);
		void orbit(float u, float v, float dist);


		float m_fov_angle;
		float m_near;
		float m_far;

		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 target_position = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up_vector = glm::vec3(0.0f, 0.0f, 1.0f);


		glm::mat4 projection = glm::mat4(1.0f);
	private:

		/* add your private declarations */
	};
} // end namespace

