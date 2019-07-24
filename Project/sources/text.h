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
#include "tbuffer.h"
#include <camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

extern glm::mat4 per_proj, view, model, ort_proj;
extern int scrWidth, scrHeight;
extern ArcBallCamera mCamera;

struct quad {
	float x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };
};

class Printer {
	const static int MAX_SLOTS = 4;
	unsigned int vbo, vao, ebo;
	Shader* shader;
	//stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
	//GLuint ftex;
	Font system;
	Font alts[MAX_SLOTS];
	int slot{ 0 };
private:
	void _initfont();
	void _initSecondaryFont();
	Font* getFont(int fid);
public:
	int load_extra_font(const char* pdesc, const char* ptex);
	Printer(Shader* s, Color c);

	~Printer() {
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, &this->ebo);
	}
	//void render(std::string str);

	void print_to_viewport(const std::string& str, quad vp, int px, int py, int fid = -1);
	void print_to_viewport(const std::string& str, quad vp, int px, int py, Color color, int fid = -1);

	void print_to_screen(const std::string& str, int px, int py, int fid = -1);
	void print_to_screen(const std::string& str, int px, int py, Color color, int fid = -1);

	void print_to_world(const std::string& str, float px, float py, float pz, Color color, int fid = -1);
	void print_to_world(const std::string& str, float px, float py, float pz, int fid = -1);

	Printer(const Printer&) = default;
	Printer& operator=(const Printer&) = default;
	void set_text_color(Color c);

	int get_str_length(const std::string& str, int fid = -1){
		int len = 0;
		auto font = getFont(fid);
		for(const auto &c : str)
		{
			len += font->characters[c].xadvance;
		}
		return len;
	}

	int get_char_advance(const char& c = 'A', int fid = -1) {
		auto font = getFont(fid);
		return font->characters[c].xadvance;
	}

	int get_font_height(int fid = -1){
		auto font = getFont(fid);
		return font->lspacing;
	}

	void get_glyph(int glyph, CharacterQuad* q) {
		Character chr = system.characters[glyph];
		get_char_quad(q, chr, 0, 0);
	}

	const Color _default, _highlighted;
};

