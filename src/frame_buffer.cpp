#include "frame_buffer.h"

namespace Orbiter
{
	FrameBuffer::FrameBuffer()
	{
		
	}

	void FrameBuffer::init(int width, int height)
	{
		printf("\t ---Init FrameBuffer\n");

		
		framebuffer_width = width;
		framebuffer_height = height;

		// fbo_width and fbo_height are the desired width and height of the FBO.
		// For Opengl <= 4.4 or if the GL_ARB_texture_non_power_of_two extension
		// is present, fbo_width and fbo_height can be values other than 2^n for
		// some integer n.

		// Build the texture that will serve as the color attachment for the framebuffer.
		GLuint texture_map;
		glGenTextures(1, &texture_map);
		glBindTexture(GL_TEXTURE_2D, texture_map);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		// Build the texture that will serve as the depth attachment for the framebuffer.
		GLuint depth_texture;
		glGenTextures(1, &depth_texture);
		glBindTexture(GL_TEXTURE_2D, depth_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Build the framebuffer.
		GLuint framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_map, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf(" ---> ERROR !!!!!!!!!!\n");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


	}

	void FrameBuffer::bind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		
		//GLCall(glViewport(0, 0, framebuffer_width, framebuffer_height));
	}

	void FrameBuffer::unbind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		
	}


} // end namespace