#pragma once

#include "pch.h"
#include "core.h"
#include "3d_utils.h"
namespace Orbiter
{
class TrajectoryDisplay : public Entity3d
{
	public:
		TrajectoryDisplay();
		
		void init();
		void update();
		void render(Camera &camera);
		
		PolylineObject polyline_object;	
	private:
	
		float m_mass;
		float m_radius;

		glm::vec3 m_velocity;
		Shader m_shader;
		Texture m_texture;
		
			
		//~ unsigned int m_vbo;
		//~ unsigned int m_vao; // vertex array object !!!!
		//~ unsigned int m_ibo;
		//~ Mesh m_mesh;

		
};
} /* end namespace */


