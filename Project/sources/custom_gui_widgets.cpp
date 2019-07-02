#include "custom_gui_widgets.h"
//---------------------------------------------------------
// RENDER VIRTUAL FUCNTIONS
//---------------------------------------------------------
// HELP TEXTBOX
void cHelpText::render(Shader * s)
{
	Form::render(s);
	if(painter) painter->print_to_screen(text, x, y); //to do: get skip line length
}

// POPUP MENU
void cPopupMenu::render(Shader * s)
{
	Form::render(s);
}

// HIGHTLIGHT SELECTOR
void cHightLightBox::render(Shader * s)
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
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
}