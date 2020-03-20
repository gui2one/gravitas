#include "scene_background.h"

namespace Orbiter
{
SceneBackground::SceneBackground()
{
	
}

void SceneBackground::init()
{
	m_texture.init();
	m_texture.load_async("images/TychoSkymapII.t3_04096x02048.jpg");
	
	if( m_vao )
	{
		GLCall(glDeleteVertexArrays(1, &m_vao));
	}
	
	GLCall(glGenVertexArrays(1, &m_vao));
	GLCall(glBindVertexArray(m_vao));	
	
	
	//~ m_mesh = Utils::makeQuadSphere(10);
	m_mesh = Utils::makeSimpleSphere(35,30);
	Utils::scaleMesh(m_mesh, glm::vec3( 2.0f, 2.0f, 2.0f));
	//~ Utils::printMeshData(m_mesh);
	m_shader.loadVertexShaderSource("../../src/res/shaders/scene_background_shader.vert");
	m_shader.loadFragmentShaderSource("../../src/res/shaders/scene_background_shader.frag");
	m_shader.createShader();
	
	
	
	if( m_vbo )
	{
		GLCall(glDeleteBuffers(1, &m_vbo));
	}
	GLCall(glGenBuffers(1, &m_vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));	

	GLCall(glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * m_mesh.vertices.size(), m_mesh.vertices.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));	
		
	if( m_ibo )
	{
		GLCall(glDeleteBuffers(1, &m_ibo));
	}
	
	GLCall(glGenBuffers(1, &m_ibo));	
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));	
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int) * m_mesh.indices.size(), m_mesh.indices.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));			
}

void SceneBackground::applyTransforms()
{

}	

void SceneBackground::render(Camera& camera)
{
	glm::mat4 temp = glm::mat4(1.0f);
	temp = glm::translate(temp, camera.position);
	
	temp = glm::rotate(temp, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_transforms = temp; 
	
	
	
	
	m_shader.useProgram();
	GLCall(glBindTexture(GL_TEXTURE_2D, m_texture.getID()));
	//~ const GLchar * uProjection = "uProjection";
	GLint projection_loc = glGetUniformLocation(m_shader.m_id, "uProjection");
	//~ printf("projection Loc : %d\n", (int)projection_loc);
	
	
	GLint model_loc = glGetUniformLocation(m_shader.m_id, "model");
	GLint view_loc = glGetUniformLocation(m_shader.m_id, "view");
	
	glm::mat4 proj_matrix = camera.projection;
	GLCall(glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(proj_matrix)));
	
	
	glm::mat4 view = glm::lookAt(camera.position, camera.target_position, camera.up_vector);
	GLCall(glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view)));

	glm::mat4 model = m_transforms;
	GLCall(glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)));
	
	GLCall(glPointSize(5.0f));

	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));	
	
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),  0 ));	
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float) )));
	GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float) )));
	GLCall(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (10 * sizeof(float) )));
	//~ GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)3));
			

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));	
	GLCall(glEnableVertexAttribArray(2));	
	GLCall(glEnableVertexAttribArray(3));	

	

	
	
	//~ GLCall(glDrawElements(GL_LINE_LOOP ,3, GL_UNSIGNED_INT, NULL));
	GLCall(glDrawElements(GL_TRIANGLES ,m_mesh.indices.size(), GL_UNSIGNED_INT, NULL));
	//~ GLCall(glDrawArrays(GL_POINTS ,0, m_mesh.vertices.size()));
	
	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));
	GLCall(glDisableVertexAttribArray(2));
	GLCall(glDisableVertexAttribArray(3));
	
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glUseProgram(0));	
}

} // end namespace Orbiter


