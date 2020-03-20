#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb_image.h"
//~ #include "vendor/stb_image_write.h"
Texture::Texture():data(),id(0),width(0), height(0), bpp(0)
{	
	//~ std::cout << "creating texture object... " << "\n";	
}

Texture::Texture(const Texture& other):
	data(other.data),
	id(other.id),
	width(other.width), 
	height(other.height),
	bpp(other.bpp)
	
{
	printf("copy texture ...\n");
}


void Texture::init()
{
	if(id) glDeleteTextures(1, &id);
	glGenTextures(1, &id);	
}

void Texture::load(std::string path)
{
	
	if(id) glDeleteTextures(1, &id);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	stbi_set_flip_vertically_on_load(1);
	unsigned char* buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	//~ async_load = std::async(std::launch::async, stbi_load, path.c_str(), &width, &height, &bpp, 4);
	
	//~ unsigned char* buffer = async_load.get();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	
	if( b_use_mipmap )
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if( buffer == nullptr)
	{
		std::cout << "stbi_failure_reason()\n";
		std::cout << stbi_failure_reason() << "\n";
		is_valid = false;
	}
	
	if(buffer)
	{
		data = std::vector<unsigned char>(buffer, buffer + width * height * 4);
		stbi_image_free(buffer);	
		is_valid = true;
	}
}

void Texture::load_async(std::string path)
{
	//~ auto fut = std::async(std::launch::async, [path, this](){
		//~ stbi_set_flip_vertically_on_load(1);
		//~ int width, height, bpp;
		//~ unsigned char* buffer = stbi_load(path.c_str() , &width, &height, &bpp, 4);
		//~ this->setData(width, height, buffer);
		//~ });
	stbi_set_flip_vertically_on_load(1);		
	async_load = std::async(std::launch::async, stbi_load, path.c_str(), &width, &height, &bpp, 4);

	
	unsigned char* buffer = async_load.get();	
	setData(width, height, buffer);
}

void Texture::setData(int _width, int _height, unsigned char* buffer, int _bpp)
{
	
	//~ glDeleteTextures(1, &id);
	//~ if(id != 0)
	//~ {
		//~ glDeleteTextures(1, &id);
	//~ }
	//~ glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buffer[0]);

	if( b_use_mipmap)	
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		
		
	glBindTexture(GL_TEXTURE_2D, 0);	
	
	
	data.clear();
	data = std::vector<unsigned char>(buffer, buffer + _width * _height * _bpp);
	
	is_valid = true;
	width = _width;
	height = _height;
	setBPP(_bpp);
	//~ std::cout << "__ setting texture data : " << _width << "/" << _height  << "\n";
}

void Texture::bind(){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	//~ std::cout << "deleting texture -- ID : " << id << "\n";
	glDeleteTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

