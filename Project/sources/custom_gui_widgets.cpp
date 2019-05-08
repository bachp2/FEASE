#include "custom_gui_widgets.h"

bool GUIForm::isHover(int mx, int my)
{

	return false;
}

void GUIForm::render(Shader * s)
{
	s->use();
	s->setMat4("model", glm::mat4(1.0f));
	s->setMat4("projection", orthogonal_projection);
	s->setColor("color", configTable.getColor("grid"));
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}