#include "custom_gui_widgets.h"
// GENERIC GUIFORM
void Form::render(Shader * s)
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
bool Form::hit_test(int mx, int my)
{
	int x0 = x; int x1 = x + width;
	int y0 = y; int y1 = y + height;

	if (mx > x1 || mx < x0) return false;
	if (my > y1 || my < y0) return false;
	//printf("form is hovered\n");
	return true;
}

void Form::move(float dx, float dy)
{
	x += dx;
	y += dy;
}

void Form::resize()
{
	//glfwMakeContextCurrent(window);
	float vertices[] = {
		0, 0, 0.0f,
		width, height, 0.0f,
		0, height, 0.0f,
		width, 0, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, 
		0, 3, 1 
	}; 

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glfwMakeContextCurrent(NULL);
}