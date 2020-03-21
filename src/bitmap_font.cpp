#include "bitmap_font.h"

namespace Orbiter
{
	

BitmapFont::BitmapFont()
{
	
}



void BitmapFont::init(Shader& shader)
{
	
	m_shader = &shader;


	unsigned char data[4] = { 255, 255, 0, 255};
	m_texture.setWrap(GL_CLAMP_TO_EDGE);
	m_texture.setMagFilter(GL_LINEAR);
	m_texture.setMinFilter(GL_LINEAR);
	m_texture.setUseMipmap(false);


	m_texture.setData(1,1, data);
	

	int error = FT_Init_FreeType( &m_library );
	
	if ( error )
	{
		printf("Error initializing Freetype2\n");
	}	
	
	//~ error = 	FT_New_Face(m_library, "./share/fonts/november/novem___.ttf", 0, &m_face);
	error = 	FT_New_Face(m_library, "share/fonts/unispace/unispace rg.ttf", 0, &m_face);
	
	if( error == FT_Err_Unknown_File_Format)
	{
		printf("--- Unknown Font Format\n");
	}else if(error)
	{
		printf("--- Error Reading Font File\n");
	}	
	
	error = FT_Set_Char_Size(
			  m_face, /* handle to face object           */
			  0,      /* char_width in 1/64th of points  */
			  m_font_size*64,  /* char_height in 1/64th of points */
			  72,     /* horizontal device resolution    */
			  72 );   /* vertical device resolution      */	
	

}

FT_Vector BitmapFont::calcTextureSize()
{
	FT_Vector ft_vec;
	
	long int width = 0;
	long int height = 0;
	long int height_max = 0;
	unsigned int error;
	unsigned int num_chars = strlen((const char *)m_text);
	for(size_t i = 0; i < num_chars; i++)
	{
		error = FT_Load_Char( m_face, m_text[i], FT_LOAD_RENDER );
		if ( error )
		{
			printf("ERROR  loading Char ( freetype )\n");
		}else{
			
			
			width += m_face->glyph->advance.x / 64;
			height += m_face->glyph->advance.y / 64;
			
			int full_height = m_face->glyph->bitmap.rows + (  m_face->glyph->bitmap.rows - m_face->glyph->bitmap_top );
			if(full_height > height_max)
			{
				height_max = full_height;
			}
		}
		
		
	}	
	
	height += height_max;
	
	height = (m_face->ascender/64) - (m_face->descender/64);
	
	ft_vec.x = width;
	ft_vec.y = height;
	
	return ft_vec;
}

FT_Vector BitmapFont::calcTextureSizeFromAtlas()
{
	FT_Vector ft_vec;

	long int width = 0;
	long int height = 0;
	long int height_max = 0;	
	unsigned int num_chars = strlen((const char *)m_text);
	for (size_t i = 0; i < num_chars; i++)
	{
		if (m_text[i] > m_atlas->getGlyphsInfos().size())
		{
			printf("char is not present in atlas, try adding more Glyphs\n");
		}else{

			glyph_info info = m_atlas->getGlyphsInfos()[m_text[i]];
			width += info.advance;
			

			int full_height = info.bitmap_rows + (info.bitmap_rows - info.y_off);
			if (full_height > height_max)
			{
				height_max = full_height;
			}
		}


	}

	height += height_max;

	height = m_atlas->atlas_face_height;

	ft_vec.x = width;
	ft_vec.y = height;

	return ft_vec;
}

void BitmapFont::renderTexture()
{
	if( m_text == nullptr)
	{
		setText("default text");
	}
	
	m_texture_size = calcTextureSize();
	//~ printf("\ntexture size = %ld X %ld\n\n", m_texture_size.x, m_texture_size.y);
	
	Mesh quad_mesh = Utils::makeSimpleQuad((1.0f / ((float)m_texture_size.y / (float)m_texture_size.x)) * m_font_size , 1.0f  * m_font_size);
	
	m_mesh_object.setMesh(quad_mesh);	
	
	unsigned int n_pixels = m_texture_size.x * m_texture_size.y;

	std::vector<unsigned char> pixels;
	pixels.reserve(n_pixels * 4);

	
	for(size_t i =0; i<n_pixels; i++)
	{
		pixels.emplace_back(0);
		pixels.emplace_back(0);
		pixels.emplace_back(0);
		pixels.emplace_back(0);

	}
	
	
	
	unsigned int error;
	unsigned int num_chars = strlen((const char *)m_text);
	
	long int cursor_x = 0;
	long int cursor_y = 0;
	for(size_t i = 0; i < num_chars; i++)
	{	
		error = FT_Load_Char( m_face, m_text[i], FT_LOAD_RENDER );
		if ( error )
		{
			printf("--- Error Loading Char\n");
		}
		

				
		long int char_start_x = m_face->glyph->bitmap_left;
		long int char_start_y = m_face->glyph->bitmap_top;
		
		FT_Bitmap ft_bitmap = m_face->glyph->bitmap;
		int ft_rows = ft_bitmap.rows;
		int ft_width = ft_bitmap.width;	

				
		long int advance_x = m_face->glyph->advance.x / 64;
		
		cursor_x += char_start_x;
		
		if(cursor_x < 0)
			cursor_x = 0;
			
		cursor_y = char_start_y;
		
		for(int y = 0; y < ft_rows; y++)
		{
			for(int x = 0; x < ft_width; x++)
			{
				unsigned int index = (y * ft_width) + x;
				unsigned char value = ft_bitmap.buffer[index];
				
				
				int offset_y = (char_start_y - m_texture_size.y) - (m_face->descender/64);
				unsigned int pix_index = ((y - offset_y)* m_texture_size.x) + (cursor_x + x);
				
				
				if( pix_index < 0 || pix_index > (n_pixels-1)){
						//~ printf("/");
						continue;
				}else{
					//~ printf("%d,", pix_index);
					pixels[(pix_index*4)+0] = 255;
					pixels[(pix_index*4)+1] = 255;
					pixels[(pix_index*4)+2] = 255;
					pixels[(pix_index*4)+3] = value;
				}
				
			}
			//~ printf("\n");
			
			cursor_y += 1;

		}		
		
		cursor_x += advance_x - char_start_x;
		
		
		
		//~ cursor_y += char_start_y;
		

		
		
	}
	
	m_texture.setData( m_texture_size.x, m_texture_size.y, pixels.data());
	

}

void BitmapFont::print_glyph_info(glyph_info& info)
{
	

	printf("char :  %c\n", info.ch);
	printf("\tx0 :  %d\n", info.x0);
	printf("\tx1 :  %d\n\n", info.x1);

	printf("\ty0 :  %d\n", info.y0);
	printf("\ty1 :  %d\n\n", info.y1);

	printf("\tx_off :  %d\n", info.x_off);
	printf("\ty_off :  %d\n\n", info.y_off);

	printf("\tadvance :  %d\n\n", info.advance);

	printf("\tbitmap_idth :  %d\n", info.bitmap_width);
	printf("\tbitmap_rows :  %d\n", info.bitmap_rows);


	for (int y = 0; y < info.bitmap_rows; y++)
	{
		for (int x = 0; x < info.bitmap_width; x++)
		{
			int index = ((y+info.y0) * m_atlas->atlas_width) + (info.x0)+ x;
			if (m_atlas->pixels[index] < 100) {
				printf("-");
			}
			else {
				printf("0");
			}
		}
		printf("\n");

	}
}

void BitmapFont::renderTextureFromAtlas()
{
	//printf("render from atlas\n");

	if (m_text == nullptr)
	{
		setText("default text");
	}

	FT_Vector tex_size = calcTextureSizeFromAtlas();

	Mesh quad_mesh = Utils::makeSimpleQuad((1.0f / ((float)tex_size.y / (float)tex_size.x)) * m_atlas->getFontSize(), 1.0f  * m_atlas->getFontSize());

	m_mesh_object.setMesh(quad_mesh);

	//printf("tex size x : %d\n", tex_size.x);
	//printf("tex size y : %d\n", tex_size.y);

	unsigned int n_pixels = tex_size.x * tex_size.y;

	std::vector<unsigned char> pixels_from_atlas;
	pixels_from_atlas.reserve(n_pixels * 4);
	for (size_t i = 0; i < n_pixels * 4; i++) {

		pixels_from_atlas.emplace_back(0);
	}

	int num_chars = strlen(m_text);
	
	long int cursor_x = 0;
	long int cursor_y = 0;

	auto infos = m_atlas->getGlyphsInfos();

	for (size_t i = 0; i < num_chars; i++)
	{

		glyph_info info = infos[m_text[i]];

		long int char_start_x = info.x_off;
		long int char_start_y = info.y_off;

		
		int ft_rows = info.bitmap_rows;
		int ft_width = info.bitmap_width;


		long int advance_x = info.advance;

		cursor_x += char_start_x;

		if (cursor_x < 0)
			cursor_x = 0;

		cursor_y = char_start_y;

		for (int y = 0; y < ft_rows; y++)
		{
			
			for (int x = 0; x < ft_width; x++)
			{
				
				int value_index = ((y + info.y0) * m_atlas->atlas_width) + (info.x0) + x;
				unsigned char value = m_atlas->pixels[value_index];
				

				int offset_y = (char_start_y - tex_size.y) - m_atlas->descender;
				unsigned int pix_index = ((y - offset_y)* tex_size.x) + (cursor_x - char_start_x + x);
				


				if (pix_index < 0 || pix_index >(n_pixels - 1)) {
					//printf("bad pixels index\n");
					continue;
				}
				else {
					//printf("%d,", pix_index);
					pixels_from_atlas[(pix_index * 4) + 0] = 255;
					pixels_from_atlas[(pix_index * 4) + 1] = 255;
					pixels_from_atlas[(pix_index * 4) + 2] = 255;
					pixels_from_atlas[(pix_index * 4) + 3] = value;

					//pixels_from_atlas.push_back(255);
					//pixels_from_atlas.push_back(255);
					//pixels_from_atlas.push_back(255);
					//pixels_from_atlas.push_back(value);
				}

			}
			//printf("\n");

			cursor_y += 1;

		}

		//printf("\n");

		cursor_x += advance_x - char_start_x;



		//~ cursor_y += char_start_y;




	}

	m_texture.setData(tex_size.x, tex_size.y, pixels_from_atlas.data());

}

void BitmapFont::setText(const char * text)
{
	m_text = text;
}

void BitmapFont::draw(unsigned int screen_width, unsigned int screen_height)
{
	
	
	float ratio = (float)screen_width / screen_height;
	
	
	m_shader->useProgram();
	
	glm::mat4 projection = glm::ortho(
				-(screen_width / 2.0f ),
				 (screen_width / 2.0f ),
				-(screen_height/ 2.0f ), 
				 (screen_height/ 2.0f ), 
				-1.0f, 
				 1.0f);
	GLint projection_loc = glGetUniformLocation(m_shader->m_id, "uProjection");
	GLCall(glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection)));	
	
	
	float quad_width;
	if (m_atlas != nullptr) {
		quad_width = 1.0f / ((float)m_atlas->atlas_width / (float)m_atlas->atlas_height);
	}
	else {
		//quad_width = 1.0f / ((float)m_texture_size.y / (float)m_texture_size.x);
		quad_width = 1.0f / ((float)m_atlas->atlas_width / (float)m_atlas->atlas_height);
	}
	
	glm::mat4 model = glm::mat4(1.0);
	
	float align_pos_x = 0.0f;
	float align_pos_y = 0.0f;
	
	float screen_pos_x = m_position.x;
	float screen_pos_y = m_position.y;
	
	
	switch(m_halign)
	{
		case FONT_HALIGN_CENTER :
			align_pos_x = (-quad_width / 2.f) * m_font_size;
			break;
		case FONT_HALIGN_START :
			align_pos_x = 0.0f;
			break;		
		case FONT_HALIGN_END :
			align_pos_x = -quad_width *  m_font_size;
			break;				
		default :
			break;
	}
	
	switch(m_valign)
	{
		case FONT_VALIGN_BOTTOM :
			align_pos_y = 0.0f;
			break;
		case FONT_VALIGN_MIDDLE :
			align_pos_y = -0.5f * m_font_size;
			break;		
		case FONT_VALIGN_TOP :
			align_pos_y = -1.0f * m_font_size;
			break;				
		default :
			break;
	}	
	
	model = glm::translate(model, glm::vec3( align_pos_x + screen_pos_x, align_pos_y + screen_pos_y, 0.0f));
	GLint model_loc = glGetUniformLocation(m_shader->m_id, "model");
	GLCall(glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)));	
	
	m_texture.bind();
	
	m_mesh_object.render();
	
	m_texture.unbind();
	glUseProgram(0);
}

} // end namespace
