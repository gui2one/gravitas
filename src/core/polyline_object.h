#pragma once
#include "../pch.h"
#include "../core.h"
#include "../3d_utils.h"
namespace Orbiter
{
class PolylineObject
{
	public:
		PolylineObject();
		void init();
		void render();
		
		inline const Polyline& getPolyline() const
		{
			return m_polyline;
			
		} 
		
		inline void setPolyline(Polyline& poly)
		{
			m_polyline = poly;
			init();
		}
		
		
	private:
	
		unsigned int m_vbo;
		unsigned int m_vao; // vertex array object !!!!
		//~ unsigned int m_ibo;	
		
		Polyline m_polyline;
		/* add your private declarations */
};
}

