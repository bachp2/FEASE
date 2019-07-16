#include "gui.h"

inline Form* FormContainer::pop_back() {
	Form* g = gui_form_container.back();
	gui_form_container.pop_back();
	return g;
}

void FormContainer::update_widgets()
{
	static bool gui_have_been_touched = false;
	static bool popup_has_been_touched = false;
	const auto mx = mouse_listener.cx;
	const auto my = mouse_listener.cy;
	double dx, dy;

	if (mouse_listener.left_click()){
		for (FormIter it = gui_form_container.end(); it != gui_form_container.begin(); )
		{
			--it;
			if((*it)->hit_test(mx, my))
			{
				_list_bump_member(it);
				gui_have_been_touched = true;
				if ((*it)->type() == Form::Type::_POP_UP_MENU) popup_has_been_touched = true;
				break;
			}
			popup_has_been_touched = false;
			gui_have_been_touched = false;
		}
		if(!popup_has_been_touched) remove_any_popups();
	}

	if (mouse_listener.right_click_once() && !popup_has_been_touched) {
		remove_any_popups();
		this->push_back(new Popup("", mx, my, 80, 100));
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