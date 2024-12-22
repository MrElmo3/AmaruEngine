#include "Core/Render/TextRender.h"

#include <glad/glad.h>

#include "Core/Render/Render.h"
#include "Core/Render/Shader.h"
#include "Util/Logger.h"

std::map<char, Character> TextRender::Characters = std::map<char, Character>();
FT_Library TextRender::ft = FT_Library();
FT_Face TextRender::face = FT_Face();

void TextRender::Init() {
	
	if (FT_Init_FreeType(&ft)){
		Logger::Error("ERROR::FREETYPE: Could not init FreeType Library");
		return;
	}
	
	if (FT_New_Face(ft, "Assets/Fonts/Arial.ttf", 0, &face)){
		Logger::Error("ERROR::FREETYPE: Failed to load font");
		return;
	}
	else {
		FT_Set_Pixel_Sizes(face, 0, 48);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				Logger::Error("ERROR::FREETYPE: Failed to load Glyph " + c);
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
};