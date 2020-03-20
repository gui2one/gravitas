#pragma once

// opengl debugging
#include <stdio.h>
#include <iostream>
#include <csignal>

#define G21_ASSERT(x) if(!(x)) std::raise(SIGINT);

#ifdef OGL_DEBUG
#define GLCall(x) GLClearError(); \
        x;\
        G21_ASSERT(GLLogCall(#x, __FILE__, __LINE__))
        
        
#else
#define GLCall(x) x;
#endif

static void GLClearError()
{
        while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
        while( GLenum error = glGetError()){
 
				std::string error_str = "";
				
				switch(error){
					case GL_INVALID_ENUM :
						error_str = "GL_INVALID_ENUM";
						break;
					case GL_INVALID_VALUE :
						error_str = "GL_INVALID_VALUE";
						break;	
					case GL_INVALID_OPERATION :
						error_str = "GL_INVALID_OPERATION";
						break;												
					case GL_INVALID_FRAMEBUFFER_OPERATION :
						error_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
						break;		
					case GL_OUT_OF_MEMORY :
						error_str = "GL_OUT_OF_MEMORY";
						break;																								
				}
                std::cout << "[OpenGL Error] (" << error_str << ") : " << function << " " << file <<  " : " << line << std::endl;
    
                return false;
        }
    
        return true;
}

