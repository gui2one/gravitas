#include "polyline_object.h"

namespace Orbiter
{
PolylineObject::PolylineObject()
{
	
}

void PolylineObject::init()
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

	GLCall(glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex) * m_polyline.vertices.size(), m_polyline.vertices.data(), GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));	
		
		
}

void PolylineObject::render()
{

	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
	//~ GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo));	
	
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),  0 ));	
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float) )));
	GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float) )));
	GLCall(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (10 * sizeof(float) )));
	
			

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));	
	GLCall(glEnableVertexAttribArray(2));	
	GLCall(glEnableVertexAttribArray(3));	


	GLCall(glDrawArrays(GL_LINE_STRIP ,0, m_polyline.vertices.size()));
	//~ GLCall(glDrawArrays(GL_POINTS ,0, m_polyline.vertices.size()));
	
	GLCall(glDisableVertexAttribArray(0));
	GLCall(glDisableVertexAttribArray(1));
	GLCall(glDisableVertexAttribArray(2));
	GLCall(glDisableVertexAttribArray(3));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//~ GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));	
}
}


