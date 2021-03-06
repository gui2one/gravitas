#pragma once
#include "pch.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Orbiter
{
	
struct glyph_info{
	char ch;
	int x0, y0, x1, y1;
	int x_off, y_off;
	int advance;
	int bitmap_width;
	int bitmap_rows;
};

class BitmapFontAtlas
{
	public:
		BitmapFontAtlas();
		void renderAtlas();

		inline const std::vector<glyph_info>& getGlyphsInfos() const { return m_glyph_infos;  }

		inline const int getFontSize() { return m_font_size;  }
		inline void setFontSize(int _size) { m_font_size = _size; }

		unsigned char * pixels;
		unsigned int atlas_width;
		unsigned int atlas_height;
		unsigned int atlas_face_height;
		unsigned int ascender;
		unsigned int descender;


	private:
	
		FT_Library    m_ft;
		FT_Face       m_face;
		
		int m_font_size = 24;
		

		std::vector<glyph_info> m_glyph_infos;
		unsigned int m_num_glyphs = 256;
		/* add your private declarations */
};

} // end namespace

