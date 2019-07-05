#include "custom_gui_widgets.h"
void FormContainer::update_widgets()
{
	auto mx = mouse_listener.cx;
	auto my = mouse_listener.cy;
	if (mouse_listener.clickedBy(GLFW_MOUSE_BUTTON_LEFT)){
		//printf("Clicked\n");
		for (FormIter it = gui_widget_container.end(); it != gui_widget_container.begin(); )
		{
			--it;
			if((*it)->hit_test(mx, my))
			{
				(*it)->draggable = true;
				_list_bump_member(it);
				//printf("hit\n");
				mouseInteractWithWidget = true;
				return;
			}
		}
		mouseInteractWithWidget = false;
	}
	double dx, dy;
	if(mouse_listener.left_drag(&dx, &dy))
	{
		auto current_widget = gui_widget_container.back();
		//sanity check
		if(current_widget->draggable)
		{
			current_widget->move(dx, dy);
		}
	}

	if (mouse_listener.nilState()) 
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

void FormContainer::generic_hit_testing_widgets(){
	auto mx = mouse_listener.cx;
	auto my = mouse_listener.cy;
	for (FormIter it = gui_widget_container.end(); it != gui_widget_container.begin(); )
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

void FormContainer::render_widgets()
{
	Shader* s = shaderTable.shader("screen");
	for(auto& w : gui_widget_container){
		w->render(s);
	}
	if (popup_menu) popup_menu->render(s);
}

void FormContainer::_list_swap_member(FormIter& n1, FormIter& n2)
{
	Form* tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

void FormContainer::_list_bump_member(FormIter & n1)
{
	Form* tmp = *n1;
	gui_widget_container.erase(n1);
	gui_widget_container.push_back(tmp);
}