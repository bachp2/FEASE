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
#include "bm_parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

extern glm::mat4 perspective_projection, view, model, orthogonal_projection;

class TextPainter {
	
	struct Font
	{
		const uint32_t size = 13;
		const uint32_t atlasWidth = 512;
		const uint32_t atlasHeight = 512;
		const uint32_t oversampleX = 4;
		const uint32_t oversampleY = 4;
		const uint32_t firstChar = ' ';
		const uint32_t charCount = '~' - ' ';
		stbtt_fontinfo info;
		Texture texture;
		std::unordered_map<int, Character> characters;
	};

	unsigned int vbo, vao, ebo;
	Shader* shader;
	//stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	//GLuint ftex;
	Font font;
public:
	TextPainter();

	TextPainter(Shader* s, Color c);

	//void render(std::string str);

	void writeBitmap(std::string str, int px, int py);
	//void writeBitmap(std::string str, int px, int py, Color c);
	TextPainter(const TextPainter&) = default;
	TextPainter& operator=(const TextPainter&) = default;
	void _initfont();

	void set_text_color(Color c);

	//int get_glyph_height(char _c){
	//	stbtt_aligned_quad q;
	//	float xoff = 0;
	//	float yoff = 0;
	//	stbtt_GetPackedQuad(font.charInfo.get(), font.atlasWidth, font.atlasHeight, _c-font.firstChar,
	//		&xoff, &yoff, &q,1);
	//	//printf("glyph height %.2f\n", q.y1 - q.y0);
	//	return q.y1 - q.y0;
	//}

	//int get_glyph_width(char _c){
	//	stbtt_aligned_quad q;
	//	float xoff = 0;
	//	float yoff = 0;
	//	stbtt_GetPackedQuad(font.charInfo.get(), font.atlasWidth, font.atlasHeight, _c-font.firstChar,
	//		&xoff, &yoff, &q,1);
	//	return q.x1 - q.x0;
	//}

	int get_font_line_gap(){
		//return get_glyph_height('A') + 2;;
		return 0;
	}
	
	float get_scale_for_pixel_height(unsigned int h){
		//return h*1.0f/get_glyph_height('A');
		return 0;
	}

	int get_line_length(const std::string& str){
		
		//unsigned int llen = 0;
		//for(auto& c : str)
		//{
		//	//printf("xadvance %d\n", font.charInfo[c - ' '].xadvance);
		//	llen += get_glyph_width(c);
		//}
		//return llen;
		return 0;
	}
	const Color _default, _highlighted;
};

