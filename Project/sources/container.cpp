#include "custom_gui_widgets.h"
void FormContainer::update_widgets()
{
	static bool gui_have_been_touched = false;
	auto mx = mouse_listener.cx;
	auto my = mouse_listener.cy;
	double dx, dy;

	if (mouse_listener.left_click()){
		for (FormIter it = gui_form_container.end(); it != gui_form_container.begin(); )
		{
			--it;
			if((*it)->hit_test(mx, my))
			{
				_list_bump_member(it);
				gui_have_been_touched = true;
				break;
			}
			gui_have_been_touched = false;
		}
	}

	if(mouse_listener.left_drag(&dx, &dy) && gui_have_been_touched)
	{
		auto current_widget = gui_form_container.back();
		current_widget->move(dx, dy);
	}

	for(const auto& w : gui_form_container)
	{
		w->update();
	}
}

bool FormContainer::generic_hit_testing_widgets(){
	auto mx = mouse_listener.cx;
	auto my = mouse_listener.cy;
	for (FormIter it = gui_form_container.end(); it != gui_form_container.begin(); )
	{
		--it;
		if((*it)->hit_test(mx, my))
		{
			return true;
		}
		return false;
	}
}

void FormContainer::render_widgets()
{
	Shader* s = shaderTable.shader("screen");
	for(auto& w : gui_form_container){
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
	gui_form_container.erase(n1);
	gui_form_container.push_back(tmp);
}