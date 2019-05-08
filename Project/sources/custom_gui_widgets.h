#pragma once
#include "shader.h"
#include "color.h"
#include "text_render.h"
#include "config_parser.h"

extern glm::mat4 perspective_projection, view, model, orthogonal_projection;
extern ConfigParser configTable;
class GUIForm {
	
	unsigned int vbo, vao, ebo;

public:
	GUIForm() {};
	GUIForm(unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h) : x(_x), y(_y), width(_w), height(_h) {
		float x0 = x;
		float y0 = y;
		float x1 = x + width;
		float y1 = y + height;

		float vertices[] = {
			x0, y0, 0.0f,
			x1, y1, 0.0f,
			x0, y1, 0.0f,
			x1, y0, 0.0f,
		};

		unsigned int indices[] = {
			0, 1, 2, 
			0, 3, 1 
		}; 

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	};

	bool isHover(int mx, int my);
	void render(Shader* s);

	unsigned int x, y, width, height;
	Color color;
};

class cButton : GUIForm
{
	std::string label;
public:
	std::string getLabel(){
		return label;
	}

	void setLabel(){
		
	}
};