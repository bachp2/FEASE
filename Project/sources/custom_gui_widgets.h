#pragma once
#include "shader.h"
#include "color.h"
#include "text_render.h"
class GUIForm {
	unsigned int x, y, width, height;
	unsigned int vbo, vao, ebo;
public:
	GUIForm(unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h) : x(_x), y(_y), width(_w), height(_h) {
		
	
	};
	bool isHover(int mx, int my);
	void render(Shader* s);
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