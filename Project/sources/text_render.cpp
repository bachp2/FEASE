#include "text_render.h"

TextPainter::TextPainter() {}

TextPainter::TextPainter(Shader * s, Color c) {
	shader = s;
	shader->use();
	shader->setColor("textColor", c);
	create_texture(&font_atlas, FPATH(resources/Bisasam.png));
	//shader->setInt("texture1", 0);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	_initfont();
}


void TextPainter::writeBitmap(std::string str, int px, int py)
{
	float xoff; float yoff;
	// assume orthographic projection with units = screen pixels, origin at top left

	std::vector<std::array<float, 5>> text_vertices;
	std::vector<std::array<unsigned int, 3>> text_indices;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ftex);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const char *text = str.c_str();
	while (*text) {
		if (*text >= 32 && *text < 128) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, 512,512, *text-32, &xoff,&yoff,&q,1);//1=opengl & d3d10+,0=d3d9

			text_vertices.push_back({q.x0,q.y0, 0.0f, q.s0,q.t0});
			text_vertices.push_back({q.x1,q.y0, 0.0f, q.s1,q.t0});
			text_vertices.push_back({q.x1,q.y1, 0.0f, q.s1,q.t1});
			text_vertices.push_back({q.x0,q.y1, 0.0f, q.s0,q.t1});
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
	nmodel = glm::scale(nmodel, glm::vec3(0.5, 0.5, 1));
	nmodel = glm::translate(nmodel, glm::vec3(px, py, 0));
	
	shader->setMat4("model", nmodel);
	shader->setMat4("view", view);
	//textShader->setMat4("projection", perspective_projection);
	shader->setMat4("projection", orthogonal_projection);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3*text_indices.size(), GL_UNSIGNED_INT, 0);
}

//void RenderText::render(std::string str) {
	//std::vector<std::array<float, 5>> text_vertices;
	//std::vector<std::array<unsigned int, 3>> text_indices;

	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//float x = 0; float y = 0;

	//float font_step = 16 * 1.0f / 256;
	//for (char& c : str)
	//{
	//	std::array<float, 5> block;
	//	float currcol = (c%16)*font_step;
	//	float currrow = (15-c/16)*font_step;

	//	block = { x, y, 0.0f, currcol, currrow }; // bottom left
	//	text_vertices.push_back(block);

	//	block = { x, y + char_size, 0.0f, currcol, currrow+font_step }; // top left
	//	text_vertices.push_back(block);

	//	block = { x + char_size, y, 0.0f, currcol+font_step, currrow }; // bottom right
	//	text_vertices.push_back(block);

	//	block = { x + char_size, y + char_size, 0.0f, currcol+font_step, currrow+font_step }; // top right
	//	text_vertices.push_back(block);

	//	x += char_size;

		/*if(x > 1.0f){
			x = -1.2;
			y -= char_size;
		}*/
	//}

	//printf("text_dat size: %d\n", text_dat.size());

	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * text_vertices.size(), &text_vertices[0], GL_DYNAMIC_DRAW);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// texture coord attribute
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//for (unsigned int i = 0; i < str.length() * 4; i += 4) {
	//	std::array<unsigned int, 3> a;
	//	a = { i,i + 1,i + 2 };
	//	text_indices.push_back(a);
	//	
	//	a = { i + 1,i + 3,i + 2 };
	//	text_indices.push_back(a);
	//}

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * text_indices.size(), &text_indices[0], GL_DYNAMIC_DRAW);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_DYNAMIC_DRAW);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, font_atlas.tex_id);

	//shader->use();

	//glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, 6 * text_indices.size() / 2, GL_UNSIGNED_INT, 0);
//}
