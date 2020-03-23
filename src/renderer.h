#pragma once

#include "pch.h"
#include "core.h"

class Renderer
{
	public:
		Renderer();
		
		void initTexture();
		void initFBO(int w, int h);
		
		void render(Orbiter::Camera &camera);
		void displayScreen();
		
		
		unsigned int m_texture, m_fbo, m_vao, m_vbo, m_ibo;
		Shader fbo_shader;
		std::vector<std::shared_ptr<Orbiter::Entity3d>> m_entities;
		
		int texture_width, texture_height;
		
		
		
	private:

		void initScreenVBO();

		/* add your private declarations */
};
