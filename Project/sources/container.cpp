#include "custom_gui_widgets.h"
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
				return;
			}
		}
		mouseInteractWithWidget = false;
	}

	if(mouse_event_listener.draggedBy(GLFW_MOUSE_BUTTON_LEFT))
	{
		auto dx = mouse_event_listener._dx;
		auto dy = mouse_event_listener._dy;
		auto current_widget = gui_widget_container.back();
		//sanity check
		if(current_widget->draggable)
		{
			mu.lock();
			current_widget->move(dx, -dy);
			mouse_event_listener._dx = 0;
			mouse_event_listener._dy = 0;
			printf("dragged %.2f, %.2f\n", dx, dy);
			mu.unlock();
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
	if (popup_menu) popup_menu->render(s);
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