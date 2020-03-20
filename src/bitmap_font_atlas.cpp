#include "bitmap_font_atlas.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write.h"


namespace Orbiter
{
BitmapFontAtlas::BitmapFontAtlas()
{
	int error;

	FT_Init_FreeType(&m_ft);
	FT_New_Face(m_ft, "./share/fonts/lato/Lato-Regular.ttf",0, &m_face);
	error = FT_Set_Char_Size(
		  m_face, /* handle to face object           */
		  0,      /* char_width in 1/64th of points  */
		  m_font_size*64,  /* char_height in 1/64th of points */
		  72,     /* horizontal device resolution    */
		  72 );   /* vertical device resolution      */	
	
	
	
	
	renderAtlas();
}

void BitmapFontAtlas::renderAtlas()
{

	int error;
	int face_width =  m_face->max_advance_width / 64;
	int face_height = m_face->max_advance_height / 64;	


	FT_Set_Char_Size(m_face, 0, m_font_size << 6, 72, 72);

	int max_dim = (1 + (m_face->size->metrics.height >> 6)) * ceilf(sqrtf(m_num_glyphs));
	int tex_width = 1;
	while (tex_width < max_dim) tex_width <<= 1;
	int tex_height = tex_width;

	// important
	atlas_width = tex_width;
	atlas_height = tex_height;
	
	int max_width = 0;
	int max_height = 0;

	int pen_x = 0, pen_y = 0;
	m_glyph_infos.clear();

	pixels = (unsigned char*)calloc(tex_width * tex_height, 255);
	for(size_t i = 0; i < m_num_glyphs; i++)
	{	
		char _ch = i;
		error = FT_Load_Char( m_face, _ch, FT_LOAD_RENDER );
		if ( error )
		{
			printf("--- Error Loading Char\n");
		}

		
		
		FT_Bitmap* bmp = &m_face->glyph->bitmap;

		if (pen_x + bmp->width >= tex_width) {
			pen_x = 0;
			pen_y += ((m_face->size->metrics.height >> 6) + 1);
		}

		for (int row = 0; row < bmp->rows; ++row) {
			for (int col = 0; col < bmp->width; ++col) {
				int x = pen_x + col;
				int y = pen_y + row;

				int index = y * tex_width + x;
				unsigned char value = bmp->buffer[row * bmp->pitch + col];
	
				pixels[index] = value;
			}
		}


		glyph_info info;
		info.ch = _ch;
		info.x0 = pen_x;
		info.y0 = pen_y;
		info.x1 = pen_x + bmp->width;
		info.y1 = pen_y + bmp->rows;

		info.x_off = m_face->glyph->bitmap_left;
		info.y_off = m_face->glyph->bitmap_top;
		info.advance = m_face->glyph->advance.x >> 6;

		info.bitmap_width = m_face->glyph->bitmap.width;
		info.bitmap_rows = m_face->glyph->bitmap.rows;


		m_glyph_infos.push_back(info);

		pen_x += bmp->width + 1;
		
	}

	atlas_face_height = (m_face->ascender / 64) - (m_face->descender / 64);
	ascender = m_face->ascender / 64;
	descender = m_face->descender / 64;

	FT_Done_FreeType(m_ft);
	unsigned char * png_pixels = (unsigned char*)calloc(tex_width * tex_height * 4, 255);

	for (size_t i = 0; i < tex_width * tex_height; i++) {

		png_pixels[(i*4) + 0] = 255;
		png_pixels[(i*4) + 1] = 255;
		png_pixels[(i*4) + 2] = 255;
		png_pixels[(i*4) + 3] = pixels[i];
	}
	stbi_write_png("./share/atlas.png",tex_width,tex_height,4, png_pixels, tex_width * 4);

	printf("-- DONE Font Atlas\n");
	//printf("wrote file\n");	
}

} // end namespace

