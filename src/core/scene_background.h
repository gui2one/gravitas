#pragma once

#include "../pch.h"
#include "../core.h"
#include "../3d_utils.h"
namespace Orbiter
{
class SceneBackground
{
	public:
		SceneBackground();
			
		void init();
		void applyTransforms();
		void render(Camera &camera);			
			
		unsigned int m_vbo;
		unsigned int m_vao; // vertex array object !!!!
		unsigned int m_ibo;
		
		glm::mat4 m_transforms;
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;		
		
		Shader m_shader;
		Texture m_texture;
		Mesh m_mesh;
					
	private:
		/* add your private declarations */
};
} // end namespace Orbiter

