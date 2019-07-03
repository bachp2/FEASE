#include "text.h"
#include <memory>
TextPainter::TextPainter(Shader * s, Color default_c) : _default(default_c), _highlighted(Color::White()) {
	shader = s;
	shader->use();
	shader->setColor("textColor", default_c);
	shader->setInt("texture1", 0);
	//shader->setInt("texture1", 0);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	_initfont();
	_initSecondaryFont();
}

TextPainter::TextPainter() : _default(Color::Black()), _highlighted(Color::White())
{
}

void TextPainter::_initfont()
{
	parse_bm_font_descriptor(FPATH(res/ms_font.fnt), &font);
	create_texture(&font.texture, FPATH(res/ms_font_0.png), false);
}

void TextPainter::_initSecondaryFont()
{
	parse_bm_font_descriptor(FPATH(res/IBM8X8.fnt), &font2);
	create_texture(&font2.texture, FPATH(res/IBM8X8_0.png), false);
}

void TextPainter::set_text_color(Color c){
	shader->setColor("textColor", c);
}

void TextPainter::print_to_screen(const std::string &str, int px, int py, Color color, int fid)
{
	shader->use();
	shader->setColor("textColor", color);
	print_to_screen(str, px, py);
}

void TextPainter::print_to_screen(const std::string &str, int px, int py, int fid)
{
	float xad = px;
	// assume orthographic projection with units = screen pixels, origin at top left
	std::vector<std::array<float, 5>> text_vertices;
	std::vector<std::array<unsigned int, 3>> text_indices;

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);*/

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const char *text = str.c_str();

	//printf("%d %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", chr.id, q.x0, q.x1, q.y0, q.y1, q.s0, q.s1, q.t0, q.t1);

	//xad += chr.xadvance;

	while (*text) {
		if (*text >= ' ' && *text < 128) {
			Character chr = font.characters[*text];
			//printf("id:%d, x:%d, y:%d, w:%d, h:%d, xoffset\n", chr.id);
			CharacterQuad q;
			get_char_quad(&q, chr, xad, py);
			//printf("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", q.x0, q.x1, q.y0, q.y1, q.s0, q.s1, q.t0, q.t1);
			text_vertices.push_back({q.x0,q.y0,0,q.s0,q.t0});
			text_vertices.push_back({q.x1,q.y0,0,q.s1,q.t0});
			text_vertices.push_back({q.x1,q.y1,0,q.s1,q.t1});
			text_vertices.push_back({q.x0,q.y1,0,q.s0,q.t1});
			xad += chr.xadvance;
		}
		++text;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * text_vertices.size(), &text_vertices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	for (unsigned int i = 0; i < str.length() * 4; i += 4) {
		std::array<unsigned int, 3> a;
		a = { i, i + 1, i + 2 };
		text_indices.push_back(a);

		a = { i, i + 2, i + 3 };
		text_indices.push_back(a);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * text_indices.size(), &text_indices[0], GL_DYNAMIC_DRAW);

	shader->use();
	glm::mat4 nmodel = glm::mat4(1.0f);
	//nmodel = glm::scale(nmodel, glm::vec3(0.5, 0.5, 1));
	//nmodel = glm::scale(nmodel, glm::vec3(scale_factor, scale_factor, scale_factor));
	//nmodel = glm::translate(nmodel, glm::vec3(px, py, 0));

	shader->setMat4("model", nmodel);
	shader->setMat4("view", view);
	shader->setMat4("projection", orthogonal_projection);

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, font.texture.tex_id);
	glDrawElements(GL_TRIANGLES, 3*text_indices.size(), GL_UNSIGNED_INT, 0);
}

void TextPainter::print_to_world(const std::string & str, float px, float py, float pz, Color color, int fid)
{
	this->shader->use();
	shader->setColor("textColor", color);
	this->print_to_world(str, px, py, pz);
}

void TextPainter::print_to_world(const std::string& str, float px, float py, float pz, int fid)
{
	float xad = 0;
	// assume orthographic projection with units = screen pixels, origin at top left
	std::vector<std::array<float, 5>> text_vertices;
	std::vector<std::array<unsigned int, 3>> text_indices;

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);*/

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const char *text = str.c_str();

	//printf("%d %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", chr.id, q.x0, q.x1, q.y0, q.y1, q.s0, q.s1, q.t0, q.t1);

	//xad += chr.xadvance;
	float x0, y0;
	while (*text) {
		if (*text >= ' ' && *text < 128) {
			Character chr = font.characters[*text];
			//printf("id:%d, x:%d, y:%d, w:%d, h:%d, xoffset\n", chr.id);
			CharacterQuad q;
			get_char_quad(&q, chr, px, py);
			//printf("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n", q.x0, q.x1, q.y0, q.y1, q.s0, q.s1, q.t0, q.t1);
			static const float _fac = 6.0f;
			q.x0 /= 128*_fac; q.x1 /= 128*_fac;
			q.y0 /= 128*_fac; q.y1 /= 128*_fac;
			x0 = (q.x1 - q.x0)/2;
			y0 = (q.y1 - q.y0)/2;
			text_vertices.push_back({xad-x0,-y0,pz,q.s0,q.t1});
			text_vertices.push_back({xad+x0,-y0,pz,q.s1,q.t1});
			text_vertices.push_back({xad+x0,y0,pz,q.s1,q.t0});
			text_vertices.push_back({xad-x0,y0,pz,q.s0,q.t0});
			/*text_vertices.push_back({q.x0,q.y0,pz,q.s0,q.t1});
			text_vertices.push_back({q.x1,q.y0,pz,q.s1,q.t1});
			text_vertices.push_back({q.x1,q.y1,pz,q.s1,q.t0});
			text_vertices.push_back({q.x0,q.y1,pz,q.s0,q.t0});*/
			xad += chr.xadvance/(128*_fac);
		}
		++text;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * text_vertices.size(), &text_vertices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	for (unsigned int i = 0; i < str.length() * 4; i += 4) {
		std::array<unsigned int, 3> a;
		a = { i, i + 1, i + 2 };
		text_indices.push_back(a);

		a = { i, i + 2, i + 3 };
		text_indices.push_back(a);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * text_indices.size(), &text_indices[0], GL_DYNAMIC_DRAW);

	shader->use();
	glm::mat4 nmodel = glm::mat4(1.0f);
	//nmodel = glm::scale(nmodel, glm::vec3(0.5, 0.5, 1));
	//nmodel = glm::scale(nmodel, glm::vec3(scale_factor, scale_factor, scale_factor));
	const float npadding = 0.001;
	px += npadding; py += npadding;
	nmodel = glm::translate(nmodel, glm::vec3(px, 0, py));

	shader->setMat4("model", nmodel);
	shader->setMat4("view", view);
	shader->setMat4("projection", perspective_projection);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, font.texture.tex_id);
	glDrawElements(GL_TRIANGLES, 3*text_indices.size(), GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
}