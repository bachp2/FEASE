#include "custom_gui_widgets.h"

bool GUIForm::isHover(int mx, int my)
{
	int x0 = x; int x1 = x + width;
	int y0 = y; int y1 = y + height;

	if (mx > x1 || mx < x0) return false;
	if (my > y1 || my < y0) return false;
	printf("form is hovered\n");
	return true;
}

void GUIForm::move(int dx, int dy)
{
	x += dx;
	y += dy;
}

void GUIForm::render(Shader * s)
{
	glDisable(GL_CULL_FACE);
	s->use();
	glm::mat4 _model = glm::mat4(1.0f);
	_model = glm::translate(_model, glm::vec3(x, y, 0));
	s->setMat4("model", _model);
	s->setMat4("projection", orthogonal_projection);
	s->setColor("color", color);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}