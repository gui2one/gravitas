#include "mesh_object.h"

namespace Orbiter
{
MeshObject::MeshObject()
{
	
}

void MeshObject::init()
{
	if( m_vao )
	{
		GLCall(glDeleteVertexArrays(1, &m_vao));
	}
	
	GLCall(glGenVertexArrays(1, &m_vao));
	GLCall(glBindVertexArray(m_vao));		
	
	
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

void MeshObject::render()
{
	
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

	GLCall(glDrawElements(GL_TRIANGLES ,m_mesh.indices.size(), GL_UNSIGNED_INT, NULL));
	//~ GLCall(glDrawArrays(GL_POINTS ,0, m_mesh.vertices.size()));
	
	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));
	GLCall(glDisableVertexAttribArray(2));
	GLCall(glDisableVertexAttribArray(3));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));	
}
}


