#pragma once

#include "pch.h"
#include "core.h"
#include "frame_buffer.h"
namespace Orbiter
{
class Renderer
{
	public:
		Renderer();

		void init(int width, int height);

		void setCamera(Camera& camera);
		void render();
		void displayScreen();
		

		
		
		unsigned int m_texture, m_fbo, m_vao, m_vbo, m_ibo;
		Shader screen_shader;
		std::vector<std::shared_ptr<Orbiter::Entity3d>> m_entities;

		int texture_width, texture_height;
		
		Camera * camera;
		
	private:

		FrameBuffer m_frame_buffer;

		
		/* add your private declarations */
};
} // end namespace