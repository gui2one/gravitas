#include "renderer.h"


namespace Orbiter
{
Renderer::Renderer()
{
}

void Renderer::init(int width, int height)
{

	m_frame_buffer.init(width, height);


	screen_shader.loadVertexShaderSource("../../src/res/shaders/screen_shader.vert");
	screen_shader.loadFragmentShaderSource("../../src/res/shaders/screen_shader.frag");
	screen_shader.createShader();



	GLCall(glGenVertexArrays(1, &m_vao));
	GLCall(glBindVertexArray(m_vao));

	if (m_vbo)
	{
		GLCall(glDeleteBuffers(1, &m_vbo));
	}
	GLCall(glGenBuffers(1, &m_vbo));

	float vertices[6 * 3 * 2] = {
		0.0, 0.0, 0.0,     0.0, 0.0,
		1.0, 0.0, 0.0,     1.0, 0.0,
		1.0, 1.0, 0.0,	   1.0, 1.0,

		0.0, 0.0, 0.0,     0.0, 0.0,
		1.0, 1.0, 0.0,     1.0, 1.0,
		0.0, 1.0, 0.0,     0.0, 1.0
	};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*3*2, vertices, GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Renderer::setCamera(Camera& _camera)
{
	camera = &_camera;
}

void Renderer::render()
{
	m_frame_buffer.bind();


	glClearColor(0.0, 0.5, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	int inc = 0;
	for (auto entity : m_entities)
	{

		printf("rendering Object %d\n", inc);
		entity->render(*camera);
		inc++;
	}


	m_frame_buffer.unbind();
}

void Renderer::displayScreen()
{

	render();


	glClearColor(0.0, 0.5, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	glBindVertexArray(m_vao);
	screen_shader.useProgram();

	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3*2, 0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3*2, (void *)(3 * sizeof(float))));
	


	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	
	

	
	GLCall(glDrawArrays(GL_TRIANGLES ,0, 6));

	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));
	
	
	GLCall(glUseProgram(0));
	glBindVertexArray(0);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	
}
} // end namespace