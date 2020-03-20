#pragma once
#include "../pch.h"

#include <thread>
#include <future>
#include <iostream>
class Texture
{
	public:
		
		Texture();
		Texture(const Texture& other);
		
		virtual ~Texture();
		void init();
		void load(std::string path);
		void load_async(std::string path);
		void setData(int width, int height, unsigned char* buffer, int bpp = 4);

		inline unsigned int getID(){ return id; }
		inline int getWidth(){ return width; }
		inline int getHeight(){ return height; }
		inline int getBPP(){ return bpp; }
		inline void setBPP(int _bpp){ bpp = _bpp;}
		
		inline void setMinFilter(GLenum val){ m_min_filter = val; }
		inline void setMagFilter(GLenum val){ m_mag_filter = val; }
		
		inline void setWrap(GLenum val){ 
			m_wrap_s = val;
			m_wrap_t = val;
		}
		inline void setWrapS(GLenum val){ m_wrap_s = val;}
		inline void setWrapT(GLenum val){ m_wrap_t = val;}
		
		void bind();
		void unbind();
		
		inline void setUseMipmap(bool _bool)
		{
			b_use_mipmap = _bool;
		}
		
		bool is_valid = false;
		std::vector<unsigned char> data;
		
		std::future<unsigned char*> async_load;
	private:
		unsigned int id = 0;

		int width, height, bpp;	
		
		
		GLenum m_min_filter = GL_LINEAR_MIPMAP_LINEAR;
		GLenum m_mag_filter = GL_LINEAR;
		
		GLenum m_wrap_s = GL_REPEAT;
		GLenum m_wrap_t = GL_REPEAT;
		
		bool b_use_mipmap = true;
	
		/* add your private declarations */
};

