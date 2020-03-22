#version 330 core

layout(location = 0) out mediump vec4 color;
in mediump vec2 f_tex_coords;
in mediump vec4 f_color;


in mediump vec3 f_normal;
in mediump vec3 f_pos;
in mediump vec3 view_dir;
uniform sampler2D u_tex; //this is the texture

uniform bool use_lighting = false;
uniform bool use_texture = true;



mediump vec3 norm;
mediump vec3 lightDir;
mediump float diff;
void main()
{          
	
	


	color = vec4(1.0, 1.0, 1.0, 1.0);
    vec3 sky_tint = vec3(.6,0.7,1.0);

	
	if( use_lighting )
	{
		norm = normalize(f_normal);
		lightDir = normalize(vec3(-2.0,-2.0,2.0) - f_pos);   
		diff = max(dot(norm, lightDir), 0.0);
		diff = clamp(diff,0.03,1.0);		
		color *= vec4(diff, diff, diff,1.0);


	}
	
	if(use_texture){
		color *= texture2D(u_tex,f_tex_coords) * 0.6;
	}
	color *= f_color;
	
    float sky_haze = pow(1.0-(dot(-norm, -normalize(view_dir))), 2.0) * 2.0;
    color += vec4(vec3(sky_haze * clamp(diff,0.02,1.0)) * sky_tint*  3.0, 1.0) ;
}




                                
                                

