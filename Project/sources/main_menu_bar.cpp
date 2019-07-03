#include "custom_gui_widgets.h"

MainMenu::MainMenu(
	std::vector<std::string> icon_names, int _x, int _y, 
	unsigned int _w, unsigned int _h, Color _c ) : Form(_x, _y, _w, _h, _c)
{
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

//int last_index = 0;
void MainMenu::update()
{
	static int last_index = 0;

	if (!hit_test(mouse_listener._cx, mouse_listener._cy)) {
		delete highlighter;
		highlighter = nullptr;
		last_index = -1;
		return;
	}
	quad q;
	int index = test_item_hit(mouse_listener._cx, mouse_listener._cy, &q);

	if (index == -1) {
		//if(highlighter) highlighter->color = hexCodeToRGB("#C0C0C0");
		//printf("yo\n");
		delete highlighter;
		highlighter = nullptr;
		//last_index = menu_items.size();
		highlight_info.highlight = false;
		last_index = -1;
	}
	//printf("hit %d item!\n", index);
	
	int x0, x1;
	for (int i = 0; i < menu_items.size(); ++i)
	{
		if(i==0) 
		{
			x0 = this->x; 
			x1 = this->x + padding.horizontal*2 + painter->get_str_length(menu_items[i]);
		}
		else {
			x0 = x1; 
			x1 = x0 + padding.horizontal*2 + painter->get_str_length(menu_items[i]);
		}
		if (i == index) break;
	}

	if(last_index != index) {
		if(highlighter){
			delete highlighter;
			highlighter = nullptr;
		}
		if(index < menu_items.size())
			highlighter = new cHightLightBox(x0, this->y, x1-x0,  text_menu_height);
		else // highlight second row
			highlighter = new cHightLightBox(q.x, q.y, q.w, q.h);
		last_index = index;
	}
	highlight_info.index = index;
	highlight_info.highlight = true;

	if(mouse_listener.clickedBy(GLFW_MOUSE_BUTTON_LEFT)){
		//mouse_event_listener.agenda = static_cast<Mouse_Agenda>(index - menu_items.size());
		switch(index-menu_items.size()){
		case 6:
			mouse_listener.agenda = Mouse_Agenda::CONNECT_ELE;
			//printf("connect element\n");
			break;
		case 7:
			mouse_listener.agenda = Mouse_Agenda::ADD_NODE;
			//printf("select node\n");
			break;
		case 8:
			mouse_listener.agenda = Mouse_Agenda::RUN_ANALYSIS;
			break;
		}
	}
}

// MAIN MENU BAR
void MainMenu::render(Shader * s)
{
	Form::render(s);
	if (highlighter != nullptr) {
		//painter->set_text_color(highlighter->textColor);
		highlighter->render(s);
	}
	auto cx = padding.horizontal;
	auto cy = 0;//to do: get skip line length
	for(const auto& str : menu_items){
		if(highlight_info.highlight) painter->print_to_screen(str, cx, cy);
		else painter->print_to_screen(str, cx, cy);
		cx += padding.horizontal*2+painter->get_str_length(str);
	}
	for(auto &a : icon_buttons){
		a.render(shaderTable.shader("texture"));
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
				x1 = this->x + padding*2 + painter->get_str_length(menu_items[i]);
			}
			else {
				x0 = x1; 
				x1 = x0 + padding*2 + painter->get_str_length(menu_items[i]);
			}

			//printf("ln: %d\n", painter->get_line_length(menu_items[i]));
			if (mx < x1 && mx > x0) 
			{
				//printf("i : %d\n", i);
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