#pragma once
#include "shader.h"
#include "color.h"
#include "create_texture.h"
#include "file_system.h"
#include <vector>
#include <array>

class RenderText {
	unsigned int vbo, vao, ebo;
	int font_size;
	Shader* shader;
	unsigned int font_atlas_id;
public:
	RenderText() {}

	RenderText(Shader* s, Color c, unsigned int cfont_size = 16);

	inline void render(std::string str, float char_size);
};

