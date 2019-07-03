#include "custom_gui_widgets.h"

Form::Form(int _x, int _y, unsigned int _w, unsigned int _h, Color _c) : 
	x(_x), y(_y), width(_w), height(_h), color(_c)
{}

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