#include "gui.h"

TextBox::TextBox(int _x, int _y, unsigned int _w, unsigned int _h, Color bkgrnd) {
	width = _w;
	height = _h;
	this->x = _x;
	this->y = _y;
	this->color = bkgrnd;

	const float bwidth = 1.0;
	const float vertices[] = {
		0,			  0, 0.0f,//0
		width,			  0, 0.0f,//1
		width,		 height, 0.0f,//2
		0,        height, 0.0f,//3
		-bwidth,       -bwidth, 0.0f,//4
		width + bwidth,       -bwidth, 0.0f,//5
		width + bwidth, height + bwidth, 0.0f,//6
		-bwidth, height + bwidth, 0.0f //7 
	};

	const unsigned int plane[] = {
		0, 2, 1, 0, 3, 2
	};

	const unsigned int border[] = {
		0, 5, 4,
		0, 1, 5, //top quad 
		3, 4, 7,
		3, 0, 4, //left quad 
		1, 6, 5,
		1, 2, 6, //right quad
		2, 7, 6,
		2, 3, 7  //bottom quad
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane), plane, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &border_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border), border, GL_DYNAMIC_DRAW);
}

void TextBox::render(Shader* s) {
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	s->use();
	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(x, y, 0));
	s->setMat4("model", _model);
	s->setMat4("projection", ort_proj);

	glBindVertexArray(vao);
	s->setColor("color", color);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	s->setColor("color", Color::Black());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, border_ebo);
	glDrawElements(GL_TRIANGLES, 8 * 4, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);

	//auto wt = 0;
	//std::string trimmed;
	//for (const auto& c : buf.container()) {
	//	/*wt += text_painter->get_char_advance(c);
	//	if (wt >= this->width) break;
	//	trimmed += c;*/
	//}

	for (const auto& s : buf.container()) {
		text_painter->print_to_screen(s, x, y); //to do: get skip line length
	}
	
}

void TextBox::update(MouseListener::Event ev) {
	if (!KeyListener::Instance->m_char) return;

	if (KeyListener::onPressed() || KeyListener::readyForHoldEvent()) {
		if (KeyListener::Instance->m_char == GLFW_KEY_ENTER)
		{
			this->buf.push_back('\n');
			KeyListener::Instance->action = GLFW_REPEAT;
			return;
		}
		this->buf.push_back(KeyListener::Instance->m_char);
		KeyListener::Instance->action = GLFW_REPEAT;
	}
}
