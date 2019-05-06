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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

extern glm::mat4 perspective_projection, view, model, orthogonal_projection;

class RenderText {
	unsigned int vbo, vao, ebo;
	Shader* shader;
	Texture font_atlas;
	unsigned char temp_bitmap[512*512];
	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	GLuint ftex;

public:
	RenderText();

	RenderText(Shader* s, Color c);

	//void render(std::string str);

	void writeBitmap(std::string str, unsigned int px, unsigned int py);

	void _initfont()
	{
		auto ttf_buffer = readFile("C:\\Users\\bachp2\\Documents\\IFEM\\FEASE\\Project\\resources\\Chicago.ttf");
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

	unsigned short get_glyph_height(char _c){
		auto c = cdata[_c - 32];
		return c.y1 - c.y0;
	}

	// costly operation, do once and store this somewhere
	unsigned short get_font_line_gap(){
		unsigned short _max = 0;
		
		for (int i = 0; i < 96; i++){
			auto g = cdata[i].y1 - cdata[i].y0;
			if (_max < g) _max = g;
		}
		return _max + 1;
	}
	
	unsigned int get_line_length(const std::string& str){
		unsigned int llen = 0;
		for(auto& c : str)
		{
			llen += cdata[c - 32].xadvance;
		}
		return llen;
	}
};

