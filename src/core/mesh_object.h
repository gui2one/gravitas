#pragma once
#include "../pch.h"
#include "../core.h"
#include "../3d_utils.h"
namespace Orbiter
{
class MeshObject
{
	public:
		MeshObject();
		void init();
		void render();
		
		inline const Mesh& getMesh() const
		{
			return m_mesh;
			
		} 
		
		inline void setMesh(Mesh& mesh)
		{
			m_mesh = mesh;
			init();
		}
		
		
	private:
	
		unsigned int m_vbo;
		unsigned int m_vao; // vertex array object !!!!
		unsigned int m_ibo;	
		
		Mesh m_mesh;
		/* add your private declarations */
};
}

