#include "gui.h"

std::vector<MenuPopupItem> parse_popup(const std::string& str, char sep, float py);
Popup::Popup(std::string structure, int _x, int _y, unsigned int _w, unsigned int _h, Color _c) : Form(_x, _y, _w, _h, _c)
{
	width = _w; height = _h; x = _x; y = _y;
	this->items = parse_popup(structure, '\n', this->y);
	if (_w - padding*2 < max_item_string_size()) {
		width = max_item_string_size() + padding*2;
	}

	auto mh = max_popup_height();
	if (mh > height) height = mh;

	const float bwidth = -1.0;
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

	const unsigned int plane[] = {
		0, 2, 1, 0, 3, 2
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &(Form::ebo));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Form::ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane), plane, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tl), tl, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &ebo[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rb), rb, GL_DYNAMIC_DRAW); 
}

Popup::~Popup() {
	printf("delted\n");
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &(Form::ebo));
	glDeleteBuffers(1, &this->ebo[0]);
	glDeleteBuffers(1, &this->ebo[1]);
	glDeleteBuffers(1, &this->ebo[2]);
	if (highlighter) {
		delete highlighter;
	}
};

void Popup::render(Shader* s){
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	s->use();
	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(x, y, 0));
	s->setMat4("model", _model);
	s->setMat4("projection", ort_proj);

	glBindVertexArray(vao);
	s->setColor("color", color);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Form::ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	s->setColor("color", Color::White());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLES, 8*2, GL_UNSIGNED_INT, 0);

	s->setColor("color", Color::Black());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLES, 8*2, GL_UNSIGNED_INT, 0);

	if (highlighter) highlighter->render(s);
	for (auto i = 0; i < items.size(); ++i) {
		if (items[i].label == "sep") {
			continue;
		}

		if(highlighter && highlighter->gety() == items[i].y)
			mPrinter->print_to_screen(items[i].label, x + padding, items[i].y, Color::White());
		else
			mPrinter->print_to_screen(items[i].label, x + padding, items[i].y, Color::Black());
	}
	mPrinter->set_text_color(Color::Black());

	glEnable(GL_DEPTH_TEST);
}

void Popup::update(MouseListener::Event ev)
{
	quad q;
	int item_index{ 0 };
	if (test_item_hit(mouse_listener.cy, &q, &item_index)) {
		if (ev.left_click()) {
			if (popup_item_has_sublevel(item_index)) {
				//printf("%s\n", get_item(item_index).sub.c_str());
				create_sub_popup(item_index);
			}
		}
		highlight_item(q);
		return;
	}

	delete_highlighter();
}

inline std::vector<MenuPopupItem> parse_popup(const std::string &str, char c, float py) {
	std::vector<MenuPopupItem> arr;
	auto size = str.size();
	auto sstart = 0;
	auto txth = mPrinter->get_font_height()+3;
	arr.reserve(10);
	for (auto i = 0; i < size; ++i) {
		MenuPopupItem mitem;
		switch (str[i]) {
		case '\n':
			mitem.label = str.substr(sstart, i - sstart);
			mitem.y = py;
			mitem.h = txth;
			arr.push_back(mitem);
			sstart = i+1;
			break;
		case '\v':
			mitem.label = str.substr(sstart, i - sstart);
			mitem.y = py;
			mitem.h = txth;
			py += mitem.h;
			arr.push_back(mitem);

			mitem = MenuPopupItem("sep");
			mitem.y = py;
			mitem.h = 5;
			arr.push_back(mitem);
			sstart = i + 1;
			break;
		case '{':
			mitem.label = str.substr(sstart, i - sstart);
			sstart = i + 1;
			int cc = 0;
			for (auto ii = i; ii < size; ++ii) {
				if (str[ii] == '{') cc++;
				if (str[ii] == '}') cc--;

				if (str[ii] == '}' && cc == 0) {
					mitem.sub = str.substr(sstart, ii - sstart);
					mitem.y = py;
					mitem.h = txth;
					sstart = ii+2;
					i = sstart;
					break;
				}
			}
			arr.push_back(mitem);
			break;
		}
		py += mitem.h;
	}

	/*for (auto& a : arr) {
		if (!a.sub.empty()) printf("%s\n", a.sub.c_str());
	}*/

	return arr;
}


inline int Popup::max_item_string_size() {
	auto max = 0;
	for (const auto& i : items) {
		auto s = mPrinter->get_str_length(i.label);
		if (s > max) max = s;
	}
	return max;
}

int Popup::test_item_hit(int my, quad* q, int* index)
{
	for (auto i = 0; i < items.size(); ++i) {
		auto y0 = items[i].y;
		auto y1 = items[i].y + items[i].h;
		if (my <= y1 && my >= y0) {
			if (items[i].label == "sep") {
				return 0;
			}
			q->x = this->x;
			q->w = this->width;
			q->y = items[i].y;
			q->h = items[i].h;
			*index = i;
			return 1;
		}
	}
	return 0;
}

void Popup::highlight_item(const quad& q)
{
	if (this->highlighter) {
		delete this->highlighter;
		this->highlighter = nullptr;
	}
	this->highlighter = new HighlightQuad(q.x, q.y, q.w, q.h);
	this->highlighter->style = HighlightQuad::Style::SOLID;
}

void Popup::delete_highlighter()
{
	if (this->highlighter) {
		delete this->highlighter;
		this->highlighter = nullptr;
	}
}

