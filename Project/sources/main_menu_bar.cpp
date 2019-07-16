#include "gui.h"

MainMenu::MainMenu(
	std::vector<std::string> icon_names, int _x, int _y, 
	unsigned int _w, unsigned int _h, Color _c ) : Form(_x, _y, _w, _h, _c)
{
	const float vertices[] = {
		0, 0, 0.0f,
		width, height, 0.0f,
		0, height, 0.0f,
		width, 0, 0.0f,
	};

	const unsigned int indices[] = {
		0, 1, 2, 
		0, 3, 1 
	}; 

	const unsigned int border[] = {1, 2};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &b_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border), border, GL_STATIC_DRAW);

	icon_buttons.reserve(icon_names.size());
	Texture* separator = nullptr;
	auto yy = this->y + text_menu_height + padding.vertical;
	for (int isize = 24, xx = padding.icon, i = 0; i < icon_names.size(); i++) {
		TextureQuad tq;
		if(icon_names[i] == "separator")	
		{
			xx += 5;
			tq = TextureQuad(xx, yy, separator_width, isize);
			xx += separator_width+5;
			if (!separator) separator = new Texture(ICON_FOLDER + icon_names[i] + ".png", false);
			tq.set_texture_ptr(separator);
		}
		else
		{
			tq = TextureQuad(xx, yy, isize, isize);
			xx += isize+padding.icon;
			tq.set_texture_ptr(new Texture(ICON_FOLDER + icon_names[i] + ".png", false));
		}
		icon_buttons.push_back(tq);
	}
}

MainMenu::~MainMenu()
{
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &(Form::ebo));
	glDeleteBuffers(1, &this->b_ebo);
	if (highlighter) delete highlighter;
}

void MainMenu::update()
{
	static int last_index = 0;
	//if (popup && popup->hit_test(mouse_listener.cx, mouse_listener.cy)) {
	//	quad q;
	//	int item_index{ 0 };
	//	if (popup->test_item_hit(mouse_listener.cy, &q, &item_index)) {
	//		if (mouse_listener.left_click_once()) {
	//			if (popup->popup_item_has_sublevel(item_index)) {
	//				//printf("%s\n", popup->get_item(item_index).sub.c_str());
	//				popup->create_sub_popup(item_index, 0);
	//			}
	//		}
	//		popup->highlight_item(q);
	//	}
	//	else popup->delete_highlighter();
	//	return;
	//}
	//else if (popup)
	//	popup->delete_highlighter();

	if (!this->hit_test(mouse_listener.cx, mouse_listener.cy)) {
		/*if (popup && mouse_listener.left_click_once()) {
			delete popup;
			popup = nullptr;
		}*/
		/*if (!popup) {
			delete highlighter;
			highlighter = nullptr;
		}*/
		last_index = -1;
		return;
	}

	quad q;
	int index = test_item_hit(mouse_listener.cx, mouse_listener.cy, &q);
	//printf("hit %d item!\n", index);
	/*if ((index == -1 || index >= menu_items.size()) && popup) {
		return;
	}*/

	if (last_index != index && index != -1) {
		//updatePopup(index, q);

		if (highlighter) {
			delete highlighter;
			highlighter = nullptr;
		}
		highlighter = new HighlightQuad(q.x, q.y, q.w, q.h);

		last_index = index;
	}

	if (mouse_listener.left_click_once()) {
		//TODO investigate memory leak
		//highlighter->shift();
		updatePopup(index, q);
		mouse_listener.agenda = static_cast<Mouse_Agenda>(index - menu_items.size());
	}

	/*highlight_info.index = index;
	highlight_info.highlight = true;*/
}

// MAIN MENU BAR
void MainMenu::render(Shader * s)
{
	//Form::render(s);
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	s->use();
	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(x, y, 0));
	s->setMat4("model", _model);
	s->setMat4("projection", orthogonal_projection);
	

	glBindVertexArray(vao);
	s->setColor("color", color);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	s->setColor("color", Color::Black());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b_ebo);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);

	if (highlighter != nullptr) {
		//painter->set_text_color(highlighter->textColor);
		highlighter->render(s);
	}
	auto cx = padding.horizontal;
	auto cy = 0;//to do: get skip line length
	
	for(const auto& str : menu_items){
		text_painter->print_to_screen(str, cx, cy);
		cx += padding.horizontal*2+text_painter->get_str_length(str);
	}
	auto ss = shaderTable.shader("texture");
	for(auto &a : icon_buttons){
		a.render(ss);
	}
}

void MainMenu::move(float _x, float _y)
{
	// empty to make this object immovable
}

int MainMenu::test_item_hit(int mx, int my, quad* q)
{
	int x0, x1, y0, y1;
	auto padding = 7;
	//check for text menu hit
	//y0 = 0, y1 = 2 + painter->get_font_line_gap();
	if (y0 = this->y, y1 = this->height, my <= y0 || my > y1){
		return -1;
	}

	enum {FIRST_ROW, SECOND_ROW} _RowMenu;
	if (y0 = this->y, y1 = text_menu_height, my > y0 && my < y1) _RowMenu = FIRST_ROW;
	else _RowMenu = SECOND_ROW;

	switch(_RowMenu)
	{
	case FIRST_ROW:
		// to do make this into class variable

		for (int i = 0; i < menu_items.size(); ++i)
		{
			if(i==0) 
			{
				x0 = this->x; 
				x1 = this->x + padding*2 + text_painter->get_str_length(menu_items[i]);
			}
			else {
				x0 = x1; 
				x1 = x0 + padding*2 + text_painter->get_str_length(menu_items[i]);
			}

			//printf("ln: %d\n", painter->get_line_length(menu_items[i]));
			if (mx < x1 && mx > x0) 
			{
				//printf("i : %d\n", i);
				q->x = x0;
				q->w = x1 - x0;
				q->y = this->y;
				q->h = text_menu_height;
				return i;
			}
		}
		break;
	case SECOND_ROW:
		//auto i = mx / 24;
		for (auto i = 0; i < icon_buttons.size(); ++i){
			float _x, _y; unsigned int _w, _h;
			icon_buttons[i].get_dims(&_x, &_y, &_w, &_h);
			if(_x <= mx && mx <= _x+_w && _w != 2){
				// return -1 for separator
				q->x = _x;
				q->w = _w;
				q->y = _y;
				q->h = _h;
				return i + menu_items.size();
			}
		}
	}
	return -1;
}

extern FormContainer gui_container;
void MainMenu::updatePopup(int index, quad& q)
{
	switch (index) {
	case 0:
		gui_container.remove_any_popups();
		gui_container.push_back( new Popup("New{Bonjour\nHello!\n}\nOpen\nSave\vQuit\n", q.x, q.y + q.h, 80, 100) );
		break;
	case 1:
		gui_container.remove_any_popups();
		gui_container.push_back( new Popup("", q.x, q.y + q.h, 80, 100) );
		break;
	case 2:
		gui_container.remove_any_popups();
		gui_container.push_back( new Popup("Display Grid\n", q.x, q.y + q.h, 80, 100) );
		break;
	default:
		break;
	}
}