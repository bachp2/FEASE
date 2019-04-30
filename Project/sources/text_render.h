#pragma once
#include "shader.h"
#include "color.h"
#include "create_texture.h"
#include "file_system.h"
#include <vector>
#include <array>
#include <memory>
#include <stb_truetype.h>
#include <stb_image.h>
#include <stb_image_write.h>

class RenderText {
	unsigned int vbo, vao, ebo;
	int font_size;
	Shader* shader;
	float char_size;
	Texture font_atlas;

	unsigned char temp_bitmap[512*512];

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	GLuint ftex;

public:
	RenderText();

	RenderText(Shader* s, Color c);

	void render(std::string str);
	void setCharacterSize(float a){
		char_size = a;
	};

	void writeBitmap(std::string str);

	void _initfont()
	{
		auto ttf_buffer = readFile("C:/windows/fonts/arial.ttf");
		//printf("%d\n",checkIfFileExist("C:/windows/fonts/arial.ttf"));
		stbtt_BakeFontBitmap(ttf_buffer.data(),0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
		//// can free ttf_buffer at this point
		glGenTextures(1, &ftex);
		glBindTexture(GL_TEXTURE_2D, ftex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
		//// can free temp_bitmap at this point
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//stbi_write_png("out.png", 512, 512, 1, temp_bitmap, 512);
	}
};

