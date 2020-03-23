#version 330 core

layout(location = 0) out mediump vec4 color;
in mediump vec2 f_tex_coords;

uniform sampler2D u_tex; //this is the texture



void main()
{          

	float intensity = 0.5;

	color = texture2D(u_tex,f_tex_coords) * vec4(intensity, intensity, intensity, 1.0);

	color = vec4( 0.8, 0.0, 0.3, 0.5);
}




                                
                                

