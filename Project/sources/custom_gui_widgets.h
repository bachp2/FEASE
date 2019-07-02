#pragma once
#include "shader.h"
#include "color.h"
#include "text.h"
#include "config_parser.h"
#include "mouse_listener.h"
#include "shader_manager.h"
#include <memory>
#include <list>
#include <mutex>
extern glm::mat4 perspective_projection, view, model, orthogonal_projection;
extern std::mutex mu;
extern ConfigParser configTable;
extern ShaderManager shaderTable;
extern MouseListener mouse_listener;
extern int scrWidth, scrHeight;

class Form {

public:
	Form(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#C1C1C1")) : x(_x), y(_y), width(_w), height(_h), color(_c) {

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
	virtual void update(){};
	virtual void move(float _x, float _y);
	
	void resize();

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
	std::atomic<bool> draggable = false;
	enum WidgetType{
		_FORM, _HELPER,
		_MAIN_MENU,
		_DROP_DOWN,
		_TEXTURE_QUAD,
		_POP_UP_MENU
	};
};

class FormContainer {
	using FormIter = std::list<Form*>::iterator;
	std::list<Form*> gui_widget_container;
	void _list_swap_member(FormIter& n1, FormIter& n2);
	void _list_bump_member(FormIter& n1);
	Form* popup_menu = nullptr;
public:
	void push_back(Form* g) { 
		gui_widget_container.push_back(g); 
	};
	Form* pop_back(){
		Form* g = gui_widget_container.back();
		
		//delete g;
		//g = nullptr;
		gui_widget_container.pop_back();
		//delete_this(g);
		return g;
	}

	void update_widgets();
	void render_widgets();
	void set_popup(Form* g) { popup_menu = g; }
	Form* get_popup() { return popup_menu; }
	void reset_popup() { 
		if(popup_menu) delete popup_menu; 
		popup_menu = nullptr; 
	}
	bool isPopup() { return popup_menu != nullptr; }
	const std::list<Form*>& get_container() { return gui_widget_container; };
	~FormContainer() {
		for(auto& w : gui_widget_container){
			delete w;
		}
	};
	void generic_hit_testing_widgets();
	bool mouseInteractWithWidget = false;
};

class cHightLightBox
{
public:
	cHightLightBox(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#633939")) {
		width = _w; height = _h; x = _x; y = _y; color = _c;
		float vertices[] = {
			0, 0, 0.0f,
			width, height, 0.0f,
			0, height, 0.0f,
			width, 0, 0.0f,
		};

		unsigned int indices[] = {
			0, 2,
			0, 3, 
			1, 2,
			1, 3
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

	~cHightLightBox(){
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, &this->ebo);
	};

	void render(Shader* s);
	void move(float _x, float _y){};
	const Color textColor = hexCodeToRGB("#ffffff");

	unsigned int vbo, vao, ebo;
	float x, y;
	unsigned int width, height;
	Color color;
};

class MainMenu : public Form
{
	std::vector<std::string> menu_items;
	TextPainter* painter;
	std::vector<TextureQuad> icon_buttons;
	cHightLightBox* highlighter = nullptr;
	struct { int index = 0; bool highlight = false; } highlight_info;
	static const int text_menu_height = 22;
	static const int icon_menu_height = 26;
public:
	// !! careful raw value input prone to bug >> should make into const
	MainMenu(std::vector<std::string> icon_names, int _x = 0, int _y = 0, unsigned int _w = scrWidth, unsigned int _h = text_menu_height+icon_menu_height, Color _c = hexCodeToRGB("#C0C0C0") ) : Form(_x, _y, _w, _h, _c)
	{
		icon_buttons.reserve(10);
		Texture* separator = nullptr;
		for (int isize = 24, xx = 0, i = 0; i < icon_names.size(); i++) {
			TextureQuad tq;
			if(icon_names[i] == "separator")	
			{
				xx += 5;
				tq = TextureQuad(xx, this->y+text_menu_height, 2, isize);
				xx += 5;
				if (!separator) separator = new Texture(ICON_FOLDER + icon_names[i] + ".png", false);
				tq.set_texture_ptr(separator);
			}
			else
			{
				tq = TextureQuad(xx, this->y+text_menu_height, isize, isize);
				xx += isize;
				tq.set_texture_ptr(new Texture(ICON_FOLDER + icon_names[i] + ".png", false));
			}
			icon_buttons.push_back(tq);
		}
	};

	void setPainter(TextPainter* tp){
		painter = tp;
	}

	void render(Shader* s);

	void move(float _x, float _y);

	void set_menu_items(std::vector<std::string> items){
		menu_items = items;
	}

	void update();

	int test_item_hit(int mx, int my);

	WidgetType type(){
		return _MAIN_MENU;
	}
};

class cHelpText : public Form {
public:
	cHelpText(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#FFFFCE")) : Form(_x, _y, _w, _h, _c)
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

class cPopupMenu : public Form {
public:
	cPopupMenu(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#FFFFCE")) : Form(_x, _y, _w, _h, _c)
	{};

	void render(Shader* s);

	WidgetType type(){
		return _POP_UP_MENU;
	}
private:

};

class cButton : public Form
{
	std::string label;
public:
	std::string getLabel(){
		return label;
	}

	void setLabel(){
		
	}
};

