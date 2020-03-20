#include "trajectory_display.h"

namespace Orbiter
{
TrajectoryDisplay::TrajectoryDisplay() : Entity3d(),
							m_mass(1.0f), m_radius(0.5f), 
							m_velocity  (glm::vec3(0.0f, 0.0f, 0.0f))
							
{
	
}

void TrajectoryDisplay::init()
{
	//~ m_texture.load("../images/1280px-Nasa_land_ocean_ice_8192.jpg");
	

	Polyline poly;
	std::vector<Vertex> vertices;
	vertices.reserve(10);
	for (int i = 0; i < 10; i++)	
	{
		Vertex vtx;
		vtx.position = glm::vec3((float)i, 0.0f, 0.0f);
		vertices.emplace_back(vtx);
	}
	
	poly.vertices = vertices;	
	polyline_object.setPolyline(poly);
	
	
	m_shader.loadVertexShaderSource("../../src/res/shaders/basic_shader.vert");
	m_shader.loadFragmentShaderSource("../../src/res/shaders/basic_shader.frag");
	m_shader.createShader();
	
	
}

void TrajectoryDisplay::update()
{
	
}

void TrajectoryDisplay::render(Camera &camera)
{
	applyTransforms();
	m_shader.useProgram();
	//~ GLCall(glBindTexture(GL_TEXTURE_2D, m_texture.getID()));
	
	GLint projection_loc = glGetUniformLocation(m_shader.m_id, "uProjection");
	GLint model_loc = glGetUniformLocation(m_shader.m_id, "model");
	GLint view_loc = glGetUniformLocation(m_shader.m_id, "view");
	GLint use_lighting_loc = glGetUniformLocation(m_shader.m_id, "use_lighting");
	GLint use_texture_loc = glGetUniformLocation(m_shader.m_id, "use_texture");
	
	glm::mat4 proj_matrix = camera.projection;
	GLCall(glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix)));
		
	glm::mat4 view = glm::lookAt(camera.position, camera.target_position, camera.up_vector);
	GLCall(glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view)));

	glm::mat4 model = getTransforms();
	GLCall(glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)));
	
	GLCall(glUniform1i(use_lighting_loc, false));
	GLCall(glUniform1i(use_texture_loc, false));
	
	

	//~ GLCall(glEnable(GL_LINE_SMOOTH));
	GLCall(glLineWidth(2));
	GLCall(glPointSize(3));

	// render MeshObject
	polyline_object.render();
	

	GLCall(glUseProgram(0));
}

} /* end namespace */
