#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 tex_coords;



layout(location = 4) uniform mat4 uProjection;
layout(location = 5) uniform mat4 model;
layout(location = 6) uniform mat4 view;

out vec2 f_tex_coords;





void main()
{         


	gl_Position =  uProjection * view * model * vec4(positions,1.0);	
	f_tex_coords = tex_coords;
	

}


