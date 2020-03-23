#pragma once
#include "pch.h"

namespace Orbiter
{
class FrameBuffer
{

public:

	FrameBuffer();

	void init(int width, int height);
	void bind();
	void unbind();


	GLuint rendered_texture;
	GLuint color_renderbuffer;
	GLuint depth_renderbuffer;
	GLuint framebuffer;


	int framebuffer_width;
	int framebuffer_height;
	
	

private:

};
}
