#include "custom_gui_widgets.h"

bool GUIForm::isHover(int mx, int my)
{
	int x0 = x; int x1 = x + width;
	int y0 = y; int y1 = y + height;

	if (mx > x1 || mx < x0) return false;
	if (my > y1 || my < y0) return false;
	//printf("form is hovered\n");
	return true;
}

void GUIForm::move(int dx, int dy)
{
	x += dx;
	y += dy;
}

void WidgetContainer::update_widgets()
{
	auto lw = gui_widget_container.end();
	if ((*lw)->isHover((*lw)->listener->_cx, (*lw)->listener->_cy)){
	
		
	}
	for(auto& w : gui_widget_container){
		//w->update();

		auto l = w->listener;
		if (l->draggedBy(GLFW_MOUSE_BUTTON_LEFT) && w->isHover(l->_cx, l->_cy)){
			w->draggable = true;
			WidgetDragged = true;
		}

		if(w->draggable && w->listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT)) {
			w->move(l->_dx, -l->_dy);
			l->_dx = 0;
			l->_dy = 0;
		}

		if (!w->listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT)) w->draggable = false;
	}
}


void GUIForm::update()
{
	if (listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT) && isHover(listener->_cx, listener->_cy)){
		draggable = true;
	}

	if(draggable && listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT)) {
		move(listener->_dx, -listener->_dy);
		listener->_dx = 0;
		listener->_dy = 0;
	}

	if (!listener->draggedBy(GLFW_MOUSE_BUTTON_LEFT)) draggable = false;
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