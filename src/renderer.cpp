#include "renderer.h"


Renderer::Renderer()
{

}

void Renderer::initTexture()
{

		
	if( m_texture != 0)
	{
		GLCall(glDeleteTextures(1, &m_texture));
		GLCall(glGenTextures(1, &m_texture)); // Generate one texture
	}else{
		GLCall(glGenTextures(1, &m_texture)); // Generate one texture
	}
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture)); // Bind the texture fbo_texture

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL)); // Create a standard texture with the width and height of our window

	// Setup the basic texture parameters
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	// Unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	

}


void Renderer::initFBO(int w, int h)
{
	texture_width = w;
	texture_height = h;
	
	initTexture();
	



	fbo_shader.loadVertexShaderSource("../../src/res/shaders/screen_shader.vert");
	fbo_shader.loadFragmentShaderSource("../../src/res/shaders/screen_shader.frag");
	fbo_shader.createShader();
	

	
	
	initScreenVBO();

	if(m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo); 
	}
	
	glGenFramebuffers(1, &m_fbo); // Generate one frame buffer and store the ID in fbo
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); // Bind our frame buffer

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer

	//~ glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, viewport_FBO_depth); // Attach the depth buffer fbo_depth to our frame buffer

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); // Check that status of our generated frame buffer

	if (status != GL_FRAMEBUFFER_COMPLETE) // If the frame buffer does not report back as complete
	{
		std::cout << "Couldn't create frame buffer, haaaaaa ?!" << std::endl; // Output an error to the console
		exit(0); // Exit the application
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind our frame buffer		
}





void Renderer::render(Orbiter::Camera &camera)
{





	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo)); // Bind our frame buffer for rendering 
	GLCall(glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_COLOR_BUFFER_BIT)); // Push our glEnable and glViewport states 
	


		
	float aspect = (float)texture_width / (float)texture_height;
	GLCall(glViewport(0, 0, (float)texture_width, (float)texture_height));

	//~ GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		
	GLCall(glClearColor (1.0f, 0.0f, 0.0f, 0.0f)); // Set the clear colour 
	GLCall(glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear the depth and colour buffers  		


	GLCall(glBindVertexArray(m_vao));
	for(auto entity : m_entities)
	{	

		
		entity->render(camera);
		

	}
	
	GLCall(glPopAttrib()); // Restore our glEnable and glViewport states 
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // Unbind our texture	
	GLCall(glBindVertexArray(0));

}


void Renderer::displayScreen()
{
	//if (m_vao)
	//	GLCall(glDeleteVertexArrays(1, &m_vao));

	GLCall(glGenVertexArrays(1, &m_vao));
	GLCall(glBindVertexArray(m_vao));

	initScreenVBO();

	//GLCall(glDisable(GL_CULL_FACE));
	//GLCall(glEnable(GL_BLEND));
	//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	fbo_shader.useProgram();

	GLCall(glEnable(GL_TEXTURE_2D));
	GLCall(glActiveTexture(GL_TEXTURE0));	
	glBindTexture (GL_TEXTURE_2D, m_texture);
	
	

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 * 2, 0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 * 2, (void *)(3 * sizeof(float))));


	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2));
	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 6));

	
	glDisable(GL_TEXTURE_2D);

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	
	
	GLCall(glUseProgram(0));
	
}

void Renderer::initScreenVBO()
{
	if (m_vbo)
	{
		GLCall(glDeleteBuffers(1, &m_vbo));
	}
	GLCall(glGenBuffers(1, &m_vbo));

	float vertices[6 * 3 * 2] = {
		0.0, 0.0, 0.0,     0.0, 0.0,
		1.0, 0.0, 0.0,     1.0, 0.0,
		1.0, 1.0, 0.0,     1.0, 1.0,

		0.0, 0.0, 0.0,     0.0, 0.0,
		1.0, 1.0, 0.0,     1.0, 1.0,
		0.0, 1.0, 0.0,     0.0, 1.0
	};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));

	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*3*2, vertices, GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
