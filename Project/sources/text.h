#pragma once
#include "shader.h"
#include "color.h"
#include "create_texture.h"
#include "file_system.h"
#include <vector>
#include <array>
#include <memory>
#include <stb_image.h>
#include <stb_image_write.h>
#include "bm_parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

extern glm::mat4 perspective_projection, view, model, orthogonal_projection;
extern int scrWidth, scrHeight;

class TextPainter {
	unsigned int vbo, vao, ebo;
	Shader* shader;
	//stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	//GLuint ftex;
	Font font;
	Font font2;
public:
	TextPainter();

	TextPainter(Shader* s, Color c);

	~TextPainter() {
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, &this->ebo);
	}
	//void render(std::string str);

	void print_to_screen(const std::string& str, int px, int py, int fid = 0);
	void print_to_screen(const std::string& str, int px, int py, Color color, int fid = 0);
	void print_to_world(const std::string& str, float px, float py, float pz, Color color, int fid = 0);
	void print_to_world(const std::string& str, float px, float py, float pz, int fid = 0);

	TextPainter(const TextPainter&) = default;
	TextPainter& operator=(const TextPainter&) = default;
	void _initfont();
	void _initSecondaryFont();
	void set_text_color(Color c);

	int get_str_length(const std::string& str){
		int len = 0;
		for(const auto &c : str)
		{
			len += font.characters[c].xadvance;
		}
		return len;
	}

	int get_font_height(){
		return font.lspacing;
	}

	const Color _default, _highlighted;
};

