#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 positions;
layout(location = 1) in vec2 tex_coords;

out vec2 f_tex_coords;

void main()
{
	gl_Position = 	vec4(positions,1.0);	
	f_tex_coords =  tex_coords;
}


