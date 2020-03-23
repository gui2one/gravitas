#pragma once

#include "../pch.h"
#include "../core.h"

namespace Orbiter
{


	class Entity3d
	{
	public:
		Entity3d();
		void init();
		void update();

		virtual void render(Camera &camera) {};

		void applyTransforms();

		inline void setPosition(glm::vec3 pos)
		{
			m_position = pos;
		}
		inline void setPositionX(float val)
		{
			m_position.x = val;
		}
		inline void setPositionY(float val)
		{
			m_position.y = val;
		}
		inline void setPositionZ(float val)
		{
			m_position.z = val;
		}
		inline glm::vec3 getPosition()
		{
			return m_position;
		}


		inline void setRotation(glm::vec3 vec)
		{
			m_rotation = vec;
		}
		inline void setRotationX(float degrees)
		{
			m_rotation.x = degrees;
		}
		inline void setRotationY(float degrees)
		{
			m_rotation.y = degrees;
		}
		inline void setRotationZ(float degrees)
		{
			m_rotation.z = degrees;
		}
		inline glm::vec3 getRotation()
		{
			return m_rotation;
		}
		inline glm::mat4 getTransforms()
		{
			return m_transforms;
		}
		inline void setTransforms(glm::mat4 matrix)
		{
			m_transforms = matrix;
		}

		inline void setScale(glm::vec3 scale)
		{
			m_scale = scale;
		}

		inline glm::vec3 getScale() const
		{
			return m_scale;
		}

	private:

		glm::mat4 m_transforms;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;




	};
} // end namespace