#include "gui.h"
// HIGHTLIGHT SELECTOR
HighlightQuad::HighlightQuad(int _x, int _y, unsigned int _w, unsigned int _h, float border_width)
{
	width = _w; height = _h; x = _x; y = _y;

	const float bwidth = border_width;
	const float vertices[] = {
				   0,			  0, 0.0f,//0
			   width,			  0, 0.0f,//1
			   width,		 height, 0.0f,//2
				   0,        height, 0.0f,//3
		     -bwidth,       -bwidth, 0.0f,//4
		width+bwidth,       -bwidth, 0.0f,//5
		width+bwidth, height+bwidth, 0.0f,//6
		     -bwidth, height+bwidth, 0.0f //7 
	};

	const unsigned int tl[] = {
		0, 5, 4,
		0, 1, 5, //top quad 
		3, 4, 7,
		3, 0, 4  //left quad 
	};

	const unsigned int rb[] = {
		1, 6, 5,
		1, 2, 6, //right quad
		2, 7, 6,
		2, 3, 7, //bottom quad
	}; 

	const unsigned int sol[] = {
		4, 6, 5,
		4, 7, 6
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tl), tl, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rb), rb, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sol), sol, GL_DYNAMIC_DRAW);
}

HighlightQuad::~HighlightQuad() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->ebo[0]);
	glDeleteBuffers(1, &this->ebo[1]);
}

void HighlightQuad::render(Shader * s)
{
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	s->use();
	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(x, y, 0));
	s->setMat4("model", _model);
	s->setMat4("projection", orthogonal_projection);

	glBindVertexArray(vao);

	switch (style) {
	case Style::POP:
		s->setColor("color", Color::White());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
		glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
		s->setColor("color", Color::Black());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
		glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
		glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
		break;
	case Style::PRESSED:
		s->setColor("color", Color::Black());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
		glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
		s->setColor("color", Color::White());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
		glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
		glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
		break;
	case Style::SOLID:
		s->setColor("color", Color::hex("#1C76E3"));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		break;
	}

	glEnable(GL_DEPTH_TEST);
}