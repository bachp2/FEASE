#pragma once
#include "shader.h"
#include "text.h"
#include "config_parser.h"
#include "mouse_listener.h"
#include "key_listener.h"
#include "shader_manager.h"
#include <list>

struct Color;
class ScreenPainter;
class FormContainer;
class TextureQuad;

extern glm::mat4 per_proj, view, model, ort_proj;
extern ConfigParser configTable;
extern ShaderManager shaderTable;
extern MouseListener mouse_listener;
extern int scrWidth, scrHeight;
extern ScreenPainter* text_painter;
extern FormContainer gui_container;

struct quad {
	float x{ 0 }, y{ 0 }, w{ 0 }, h{0};
};

class Form {

public:
	Form(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#C1C1C1"));
	Form();;
	virtual ~Form() {};

	bool hit_test(int mx, int my);
	virtual void render(Shader* s){};
	virtual void update(MouseListener::Event ev){};
	virtual void move(float _x, float _y);

	void resize();

	enum Type;
	virtual Type type();;
	enum Type{
		_FORM, _HELPER,
		_MAIN_MENU,
		_DROP_DOWN,
		_TEXTURE_QUAD,
		_POP_UP_MENU
	};
protected:
public:
	unsigned int vbo, vao, ebo;
	float x, y;
	unsigned int width, height;
	Color color;
};

class FormContainer {
	using FormIter = std::list<Form*>::iterator;
	std::list<Form*> gui_form_container;
	void _list_swap_member(FormIter& n1, FormIter& n2);
	void _list_bump_member(FormIter& n1);
public:
	void push_back(Form* g) { 
		gui_form_container.push_back(g); 
	};
	Form* pop_back();
	
	Form*& back() {
		return gui_form_container.back();
	}

	void update_widgets();
	void render_widgets();
	
	void resize_widgets() {
		for (auto& w : gui_form_container) {
			if (w->type() == Form::Type::_MAIN_MENU) {
				w->width = scrWidth;
				w->resize();
			}
		}
	}
	
	bool has_popup() {
		for (auto i = gui_form_container.begin(); i != gui_form_container.end(); ++i) {
			if ((*i)->type() == Form::Type::_POP_UP_MENU) {
				return true;
			}
		}
		return false;
	}

	void remove_any_popups() {
		std::list<Form*> tmp;
		for (auto i = gui_form_container.begin(); i != gui_form_container.end(); ++i) {
			if ((*i)->type() == Form::Type::_POP_UP_MENU) {
				delete (*i);
			}
			else {
				tmp.push_back(*i);
			}
		}
		gui_form_container = tmp;
	}

	~FormContainer() {
		for (auto& w : gui_form_container) {
			delete w;
		}
	};
	
	bool generic_hit_testing_widgets();
};

class HighlightQuad
{
public:
	enum Style {
		SOLID,
		POP,
		PRESSED
	};
	Style style = Style::POP;
	HighlightQuad(int _x, int _y, unsigned int _w, unsigned int _h, float border_width = -1.0);

	~HighlightQuad();;
	
	int getx() { return int(this->x); }
	int gety() { return int(this->y); }

	void render(Shader* s);
	void move(float _x, float _y){};
private:
	unsigned int vbo, vao, ebo[3];
	float x, y;
	unsigned int width, height;
};

// TODO refactor this struct inside main menu class
static struct {
	const int horizontal = 7;
	const int vertical = 2;
	const int icon = 5;
} padding;

class MainMenu : public Form
{
	std::vector<std::string> menu_items;
	std::vector<TextureQuad> icon_buttons;
	HighlightQuad* highlighter = nullptr;
	unsigned int b_ebo;
	static const int text_menu_height = 18;
	static const int icon_menu_height = 26;
	static const int separator_width = 2;
public:
	// !! careful raw value input prone to bug >> should make into const
	MainMenu(
		std::vector<std::string> icon_names, 
		int _x = 0, int _y = 0, 
		unsigned int _w = scrWidth, 
		unsigned int _h = text_menu_height + icon_menu_height + padding.vertical * 2,
		Color _c = Color::hex("#C0C0C0"));

	~MainMenu();

	void render(Shader* s);

	void move(float _x, float _y);

	void set_menu_items(std::vector<std::string> items){
		menu_items = items;
	}

	void update(MouseListener::Event ev);

	int test_item_hit(int mx, int my, quad* q);

	Type type(){
		return _MAIN_MENU;
	}
private:
	void create_popup(int index, quad& q);
};

class StaticTextMessage : public Form
{
public:
	StaticTextMessage(std::string message, int _x, int _y, Color bkgrnd = Color::hex("#FFFFCE"));
	void render(Shader* s);
private:
	std::string message;
	unsigned int border_ebo;
};

class TextBox : public Form
{
public:
	TextBox(int _x, int _y, unsigned int _w, unsigned int _h, Color bkgrnd = Color::hex("#FFFFCE"));
	void render(Shader* s);
	void update(MouseListener::Event ev);
private:
	std::string text;
	unsigned int border_ebo;
};

struct MenuPopupItem {
	MenuPopupItem() {};

	MenuPopupItem(const std::string& lab) {
		label = lab;
	};

	std::string label;
	std::string sub;
	int y{ 0 }, h{ 0 };
	//TextureQuad tq;
};

class Popup : public Form {
public:
	const static int MAXIMUM_SUBLEVEL = 3;
	Popup(std::string structure, int _x, int _y, unsigned int _w, unsigned int _h, Color _c = Color::hex("#D4D0C8"));
	~Popup();
	void render(Shader* s);
	void update(MouseListener::Event ev);
	Type type(){
		return _POP_UP_MENU;
	}

	int max_item_string_size();;
	int max_popup_height() {
		return text_painter->get_font_height() * (items.size() + 1);
	}

	int test_item_hit(int my, quad* q, int* index);
	void highlight_item(const quad& q);
	void delete_highlighter();

	bool popup_item_has_sublevel(int index) {
		return !items[index].sub.empty();
	}

	void create_sub_popup(int index) {
		const auto item = items.at(index);
		gui_container.push_back(new Popup(item.sub, x+width-2, item.y+1, 80, 50));
		printf("sublevel created\n");
	}

private:
	unsigned int ebo[2];
	const static int padding{ 15 };
	HighlightQuad* highlighter{ nullptr };
	std::vector<MenuPopupItem> items;
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

