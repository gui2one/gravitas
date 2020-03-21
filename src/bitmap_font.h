#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "pch.h"
#include "core.h"
#include "3d_utils.h"
#include "bitmap_font_atlas.h"



namespace Orbiter
{
	
enum FONT_HALIGN
{
	FONT_HALIGN_CENTER,
	FONT_HALIGN_START,
	FONT_HALIGN_END
};

enum FONT_VALIGN
{
	FONT_VALIGN_MIDDLE,
	FONT_VALIGN_BOTTOM,
	FONT_VALIGN_TOP
};

class BitmapFont
{
	public:
	//// freetype example from : https://freetype.org/freetype2/docs/tutorial/step1.html
		BitmapFont();
		
		inline void setHAlign(FONT_HALIGN enum_val) { m_halign = enum_val; }
		inline void setVAlign(FONT_VALIGN enum_val) { m_valign = enum_val; }
		
		inline void setPosition( glm::vec2 vec ){ m_position = vec; }
		inline glm::vec2 getPosition(){ return m_position; }
		
		void init(Shader& shader);
		
		void setText(const char * text = "example text");
		FT_Vector calcTextureSize();
		FT_Vector calcTextureSizeFromAtlas();
		void renderTexture();
		void print_glyph_info(glyph_info& info);
		void renderTextureFromAtlas();
		void draw(unsigned int screen_width, unsigned int screen_height);
		

		inline void setAtlas(BitmapFontAtlas& atlas) { m_atlas = &atlas; }
		inline const BitmapFontAtlas* getAtlas() { return m_atlas; }

	private:
	
	
	unsigned int m_font_size = 16;
	FT_Library  m_library;
	FT_Face     m_face;	
	Texture m_texture;
	FT_Vector m_texture_size;
	Shader * m_shader;
	MeshObject m_mesh_object;
	

	
	FONT_HALIGN m_halign = FONT_HALIGN_CENTER;
	FONT_VALIGN m_valign = FONT_VALIGN_BOTTOM;
	const char * m_text = nullptr;
	
	glm::vec2 m_position = glm::vec2(0.0, 0.0);


	BitmapFontAtlas * m_atlas = nullptr;
		/* add your private declarations */
};
} // end namespace

