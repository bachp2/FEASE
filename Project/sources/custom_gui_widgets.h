#pragma once
#include "shader.h"
#include "color.h"
#include "text_render.h"
#include "config_parser.h"
#include "mouse_listener.h"
#include "text_render.h"
#include "shader_manager.h"
#include <list>

extern glm::mat4 perspective_projection, view, model, orthogonal_projection;
extern ConfigParser configTable;
extern ShaderManager shaderTable;
extern MouseListener mouse_event_listener;
extern int scrWidth, scrHeight;

class GUIForm {

public:
	GUIForm(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#C1C1C1")) : x(_x), y(_y), width(_w), height(_h), color(_c) {

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

	bool hit_test(int mx, int my);

	virtual void render(Shader* s);
	virtual void update();
	virtual void move(float _x, float _y);
	
	void resize(){
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
	}

	enum WidgetType;
	virtual WidgetType type() 
	{
		return _FORM;
	};

public:
	unsigned int vbo, vao, ebo;
	float x, y;
	unsigned int width, height;
	Color color;
	bool draggable = false;
	enum WidgetType{
		_FORM, _HELPER,
		_MAIN_MENU,
		_DROP_DOWN
	};
};

class WidgetContainer {
	using WidgetIter = std::list<GUIForm*>::iterator;
	std::list<GUIForm*> gui_widget_container;
	void _list_swap_member(WidgetIter& n1, WidgetIter& n2);
	void _list_bump_member(WidgetIter& n1);
public:
	void push_back(GUIForm* g) { gui_widget_container.push_back(g); };
	void update_widgets();
	void render_widgets();
	const std::list<GUIForm*> get_container() { return gui_widget_container; };
	~WidgetContainer() {
		for(auto& w : gui_widget_container){
			delete w;
		}
	};
};

class cMenuBar : GUIForm
{
	std::vector<std::string> menu_items;
	TextPainter* painter;
public:
	cMenuBar(int _x = 0, int _y = 0, unsigned int _w = scrWidth, unsigned int _h = 19, Color _c = hexCodeToRGB("#C0C0C0")) : GUIForm(_x, _y, _w, _h, _c)
	{};

	void setPainter(TextPainter* tp){
		painter = tp;
	}

	void render(Shader* s);

	void move(float _x, float _y);

	void set_menu_items(std::vector<std::string> items){
		menu_items = items;
	}

	int test_item_hit(){
		
		
	}

	WidgetType type(){
		return _MAIN_MENU;
	}
};

class cHelpText : public GUIForm {
public:
	cHelpText(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#FFFFCE")) : GUIForm(_x, _y, _w, _h, _c)
	{};

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

	WidgetType type(){
		return _HELPER;
	}
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

