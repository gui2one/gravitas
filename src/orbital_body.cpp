#include "orbital_body.h"

namespace Orbiter
{
OrbitalBody::OrbitalBody() : Entity3d(),
							m_density(1.0f), m_radius(0.5f), 
							m_velocity  (glm::vec3(0.0f, 0.0f, 0.0f))
							
{
	
}

void OrbitalBody::init()
{
	//~ m_texture.load_async("../images/1280px-Nasa_land_ocean_ice_8192.jpg");
	
	m_texture.init();

	
	
	Mesh mesh = Utils::makeSimpleSphere(60,40, m_radius);
	m_mesh_object.setMesh(mesh);
	
	
	m_shader.loadVertexShaderSource("../../src/res/shaders/planet_shader.vert");
	m_shader.loadFragmentShaderSource("../../src/res/shaders/planet_shader.frag");
	m_shader.createShader();
	
	
}

void OrbitalBody::update()
{
	
}

void OrbitalBody::render(Camera &camera)
{
	applyTransforms();
	
	m_shader.useProgram();
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture.getID()));
	
	GLint projection_loc = glGetUniformLocation(m_shader.m_id, "uProjection");
	GLint model_loc = glGetUniformLocation(m_shader.m_id, "model");
	GLint view_loc = glGetUniformLocation(m_shader.m_id, "view");
	GLint use_lighting_loc = glGetUniformLocation(m_shader.m_id, "use_lighting");
	
	glm::mat4 proj_matrix = camera.projection;
	GLCall(glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix)));
		
	glm::mat4 view = glm::lookAt(camera.position, camera.target_position, camera.up_vector);
	GLCall(glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view)));

	glm::mat4 model = getTransforms();
	GLCall(glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)));
	
	GLCall(glUniform1i(use_lighting_loc, 1));
	
	GLCall(glPointSize(5.0f));

	// render MeshObject
	m_mesh_object.render();
	
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	GLCall(glUseProgram(0));
}

void OrbitalBody::setOrbitalParent(std::shared_ptr<OrbitalBody> parent_body)
{
	orbital_parent = parent_body;
}

} /* end namespace */
