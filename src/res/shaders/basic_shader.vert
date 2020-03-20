#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 positions;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec4 vertex_color;
layout(location = 3) in vec2 tex_coords;



layout(location = 4) uniform mat4 uProjection;
layout(location = 5) uniform mat4 model;
layout(location = 6) uniform mat4 view;



out vec3 f_normal;
out vec2 f_tex_coords;
out vec3 f_pos;
out vec4 f_color;



void main()
{         

	f_color = vertex_color;
	f_pos = vec3(model * vec4(positions,1.0));
	f_normal = mat3( transpose(inverse(model)) ) * normals;
	gl_Position =  uProjection * view * model * vec4(positions,1.0);	
	f_tex_coords = tex_coords;
	

}


