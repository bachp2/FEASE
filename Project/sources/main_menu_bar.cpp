#include "custom_gui_widgets.h"
int last_index = 0;
void MainMenu::update()
{
	if (!hit_test(mouse_listener._cx, mouse_listener._cy)) {
		delete highlighter;
		highlighter = nullptr;
		last_index = -1;
		return;
	}

	int index = test_item_hit(mouse_listener._cx, mouse_listener._cy);

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
	auto padding = 7;
	int x0, x1;
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
		if (i == index) break;
	}

	if(last_index != index) {
		if(index < menu_items.size())
			highlighter = new cHightLightBox(x0, this->y, x1-x0,  text_menu_height);
		else // highlight second row
			highlighter = new cHightLightBox(this->x + (index-menu_items.size())*24, text_menu_height, 24,  icon_menu_height);
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
	auto padding = 7;
	auto cx = padding;
	auto cy = 2;//to do: get skip line length
	for(const auto& str : menu_items){
		if(highlight_info.highlight) painter->print_to_screen(str, cx, cy);
		else painter->print_to_screen(str, cx, cy);
		cx += padding*2+painter->get_str_length(str);
	}
	for(auto &a : icon_buttons){
		a.render(shaderTable.getShader("texture"));
	}
}

void MainMenu::move(float _x, float _y)
{
	// empty to make this object immovable
}

int MainMenu::test_item_hit(int mx, int my)
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
		auto i = mx / 24;
		if (i >= icon_buttons.size()) return -1;
		return i + menu_items.size();
		break;
	}


	return -1;
}