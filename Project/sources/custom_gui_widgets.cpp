#include "custom_gui_widgets.h"

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

void cMenuBar::move(float _x, float _y)
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
				break;
			}
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

void GUIForm::update()
{
	/*if (listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT) && isHover(listener->_cx, listener->_cy)){
		draggable = true;
	}

	if(draggable && listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT)) {
		move(listener->_dx, -listener->_dy);
		listener->_dx = 0;
		listener->_dy = 0;
	}

	if (!listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT)) draggable = false;*/
}

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

void cHelpText::render(Shader * s)
{
	GUIForm::render(s);
	//painter->writeBitmap(text, x*2, y*2+painter->get_font_line_gap());	
	painter->writeBitmap(this->text, x*2, y*2+painter->get_font_line_gap());	
	//painter->writeBitmap(text, x*2, y*2);	
}

void cMenuBar::render(Shader * s)
{
	GUIForm::render(s);

	auto cx = x * 2;
	auto cy = y * 2 + painter->get_font_line_gap();

	painter->writeBitmap(" ", cx, cy);
	cx += painter->get_glyph_width(' ')/2;
	for(const auto& str : menu_items){
		painter->writeBitmap(str, cx, cy);
		cx += painter->get_line_length(str)/2;
	}
	
}