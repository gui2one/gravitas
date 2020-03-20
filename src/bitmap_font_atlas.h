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

		unsigned char * pixels;
	private:
	
		FT_Library    m_ft;
		FT_Face       m_face;
		
		int m_font_size = 32;
		

		std::vector<glyph_info> m_glyph_infos;
		unsigned int m_num_glyphs = 256;
		/* add your private declarations */
};

} // end namespace

