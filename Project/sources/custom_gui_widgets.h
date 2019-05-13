#pragma once
#include "shader.h"
#include "color.h"
#include "text_render.h"
#include "config_parser.h"
#include "mouse_listener.h"
#include "text_render.h"

extern glm::mat4 perspective_projection, view, model, orthogonal_projection;
extern ConfigParser configTable;
extern MouseListener mouse_event_listener;

class GUIForm {
	
public:
	GUIForm() {};
	GUIForm(unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#C1C1C1")) : x(_x), y(_y), width(_w), height(_h), color(_c) {

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

	virtual void render(Shader* s);
	virtual void update();
	void move(int _x, int _y);

public:
	unsigned int vbo, vao, ebo;
	int x, y, width, height;
	Color color;
	bool moveable = false;
	MouseListener* listener = nullptr;
};

class cHelpText : public GUIForm {
public:
	cHelpText(unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#FFFFCE")) : GUIForm(_x, _y, _w, _h, _c)
	{

	}

	cHelpText() {};

	void include_text(std::string t){
		text = t;
	};

	void setPainter(TextPainter* tp){
		painter = tp;
	}

	void clear_text() {
		text.clear();
	};

	void render(Shader* s);
private:
	std::string text;
	TextPainter* painter;
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