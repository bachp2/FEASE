#include "custom_gui_widgets.h"
//#include <mutex>
//extern std::mutex mu;


bool GUIForm::hit_test(int mx, int my)
{
	int x0 = x; int x1 = x + width;
	int y0 = y; int y1 = y + height;

	if (mx > x1 || mx < x0) return false;
	if (my > y1 || my < y0) return false;
	//printf("form is hovered\n");
	return true;
}

void GUIForm::move(float dx, float dy)
{
	x += dx;
	y += dy;
}

void cMainMenuBar::move(float _x, float _y)
{
	// empty to make this object immovable
}

void WidgetContainer::update_widgets()
{
	auto mx = mouse_event_listener._cx;
	auto my = mouse_event_listener._cy;
	if (mouse_event_listener.clickedBy(GLFW_MOUSE_BUTTON_LEFT)){
		//printf("Clicked\n");
		for (WidgetIter it = gui_widget_container.end(); it != gui_widget_container.begin(); )
		{
			--it;
			if((*it)->hit_test(mx, my))
			{
				(*it)->draggable = true;
				_list_bump_member(it);
				//printf("hit\n");
				mouseInteractWithWidget = true;
				break;
			}
			mouseInteractWithWidget = false;
		}
	}

	if(mouse_event_listener.draggedBy(GLFW_MOUSE_BUTTON_LEFT))
	{
		auto dx = mouse_event_listener._dx;
		auto dy = mouse_event_listener._dy;
		auto current_widget = gui_widget_container.back();
		//sanity check
		if(current_widget->draggable)
		{
			current_widget->move(dx, -dy);
			mouse_event_listener._dx = 0;
			mouse_event_listener._dy = 0;
		}
	}

	if (mouse_event_listener.nilState()) 
	{
		// safe check
		auto current_widget = gui_widget_container.back();
		current_widget->draggable = false;
	}

	for(const auto& w : gui_widget_container)
	{
		w->update();
	}
	//mouseInteractWithWidget = false;
}

void WidgetContainer::generic_hit_testing_widgets(){
	auto mx = mouse_event_listener._cx;
	auto my = mouse_event_listener._cy;
	for (WidgetIter it = gui_widget_container.end(); it != gui_widget_container.begin(); )
	{
		--it;
		if((*it)->hit_test(mx, my))
		{
			mouseInteractWithWidget = true;
			break;
		}
		mouseInteractWithWidget = false;
	}
}

void WidgetContainer::render_widgets()
{
	Shader* s = shaderTable.getShader("2D");
	for(auto& w : gui_widget_container){
		w->render(s);
	}
}

void WidgetContainer::_list_swap_member(WidgetIter& n1, WidgetIter& n2)
{
	GUIForm* tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

void WidgetContainer::_list_bump_member(WidgetIter & n1)
{
	GUIForm* tmp = *n1;
	gui_widget_container.erase(n1);
	gui_widget_container.push_back(tmp);
}

int last_index = 0;
void cMainMenuBar::update()
{
	if (!hit_test(mouse_event_listener._cx, mouse_event_listener._cy)) {
		delete highlighter;
		highlighter = nullptr;
		last_index = -1;
		return;
	}

	int index = test_item_hit(mouse_event_listener._cx, mouse_event_listener._cy);

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

	if(mouse_event_listener.clickedBy(GLFW_MOUSE_BUTTON_LEFT)){
		//mouse_event_listener.agenda = static_cast<Mouse_Agenda>(index - menu_items.size());
		switch(index-menu_items.size()){
		case 6:
			mouse_event_listener.agenda = Mouse_Agenda::CONNECT_ELE;
			//printf("connect element\n");
			break;
		case 7:
			mouse_event_listener.agenda = Mouse_Agenda::ADD_NODE;
			//printf("select node\n");
			break;
		case 8:
			mouse_event_listener.agenda = Mouse_Agenda::RUN_ANALYSIS;
			break;
		}
	}
}

//---------------------------------------------------------
// RENDER VIRTUAL FUCNTIONS
//---------------------------------------------------------

// GENERIC GUIFORM
void GUIForm::render(Shader * s)
{
	//glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
	s->use();
	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(x, y, 0));
	s->setMat4("model", _model);
	s->setMat4("projection", orthogonal_projection);
	s->setColor("color", color);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	
}
// HELP TEXTBOX
void cHelpText::render(Shader * s)
{
	GUIForm::render(s);
	if(painter) painter->print_to_screen(text, x, y); //to do: get skip line length
}

// MAIN MENU BAR
void cMainMenuBar::render(Shader * s)
{
	GUIForm::render(s);
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

// POPUP MENU
void cPopupMenu::render(Shader * s)
{
	GUIForm::render(s);
}

// HIGHTLIGHT SELECTOR
void cHightLightBox::render(Shader * s)
{
	GUIForm::render(s);
}