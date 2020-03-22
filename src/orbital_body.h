#pragma once

#include "pch.h"
#include "core.h"
#include "3d_utils.h"
namespace Orbiter
{
class OrbitalBody : public Entity3d
{
	public:
		OrbitalBody();
		
		void init();
		void update();
		void render(Camera &camera);
		void setOrbitalParent(std::shared_ptr<OrbitalBody> parent_body);
		
		inline void setRadius(float radius)
		{ 
			m_radius = radius;	
			init();		
		}
		inline float getRadius() { return m_radius; }

		inline void setDensity(float density){ m_density = density; }
		inline float getDensity() { return m_density; }
		

		inline Shader& getShader() { return m_shader;  }
		
		inline void loadTexture(const char * path)
		{ 
			m_texture.load_async(path);
		}
				
		std::shared_ptr<OrbitalBody> orbital_parent;
		
	private:
	
		float m_density;
		float m_radius;

		glm::vec3 m_velocity;
		Shader m_shader;
		Texture m_texture;
		
		MeshObject m_mesh_object;		
		//~ unsigned int m_vbo;
		//~ unsigned int m_vao; // vertex array object !!!!
		//~ unsigned int m_ibo;
		//~ Mesh m_mesh;

		
};
} /* end namespace */

