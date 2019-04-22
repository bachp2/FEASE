#pragma once
#include "shader.h"
#include "color.h"
#include "create_texture.h"
#include "file_system.h"
#include <vector>
#include <array>
#include <memory>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <stb_image.h>




class RenderText {
	unsigned int vbo, vao, ebo;
	int font_size;
	Shader* shader;
	float char_size;
	Texture font_atlas;

public:
	RenderText();

	RenderText(Shader* s, Color c, unsigned int cfont_size = 16);

	void loadTTF(std::string path);
	void render(std::string str);
	void setCharacterSize(float a){
		char_size = a;
	};

private:
	void _initfont(void)
	{
		fread(ttf_buffer, 1, 1<<20, fopen("c:/windows/fonts/times.ttf", "rb"));
		stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
		// can free ttf_buffer at this point
		glGenTextures(1, &ftex);
		glBindTexture(GL_TEXTURE_2D, ftex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
		// can free temp_bitmap at this point
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void _printASCII(float x, float y, char *text)
	{
		// assume orthographic projection with units = screen pixels, origin at top left
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ftex);
		glBegin(GL_QUADS);
		while (*text) {
			if (*text >= 32 && *text < 128) {
				stbtt_aligned_quad q;
				stbtt_GetBakedQuad(cdata, 512,512, *text-32, &x,&y,&q,1);//1=opengl & d3d10+,0=d3d9
				glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y0);
				glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y0);
				glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y1);
				glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y1);
			}
			++text;
		}
		glEnd();
	}

	unsigned char ttf_buffer[1<<20];
	unsigned char temp_bitmap[512*512];

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	GLuint ftex;
};

