#include "text_render.h"

RenderText::RenderText() {}

RenderText::RenderText(Shader * s, Color c, unsigned int cfont_size) {
	shader = s;
	font_size = cfont_size;
	shader->use();
	shader->setColor("textColor", c);
	create_texture(&font_atlas_id, FPATH(resources/Bisasam.png));
	shader->setInt("texture1", 0);
}

void RenderText::render(std::string str, float char_size) {
	std::vector<std::array<float, 5>> text_dat;
	std::vector<std::array<unsigned int, 3>> text_indices;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float x = 0; float y = 0;
	float font_step = 16 * 1.0f / 256;
	for (int i = 0; i < 16; ++i)
	{
		std::array<float, 5> block;
		float char_pos = i*font_step;
		if (i == 0) {
			block = { x, y, 0.0f, char_pos, 0.0f }; // bottom left
			text_dat.push_back(block);

			block = { x, y + char_size, 0.0f, char_pos, font_step }; // top left
			text_dat.push_back(block);
		}

		block = { x + char_size, y, 0.0f, char_pos + font_step, 0.0f }; // bottom right
		text_dat.push_back(block);

		block = { x + char_size, y + char_size, 0.0f, char_pos + font_step, font_step }; // top right
		text_dat.push_back(block);

		x += char_size;
	}

	//printf("text_dat size: %d\n", text_dat.size());
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * text_dat.size(), &text_dat[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	for (unsigned int i = 0; i < 16 * 2; i += 2) {
		std::array<unsigned int, 3> a;
		a = { i,i + 1,i + 2 };
		text_indices.push_back(a);

		a = { i + 1,i + 3,i + 2 };
		text_indices.push_back(a);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * text_indices.size(), &text_indices[0], GL_DYNAMIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_DYNAMIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font_atlas_id);

	shader->use();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6 * text_indices.size() / 2, GL_UNSIGNED_INT, 0);
	Shader::reset();
}
