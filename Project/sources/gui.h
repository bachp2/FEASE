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
extern TextPainter* text_painter;

struct quad {
	float x{ 0 }, y{ 0 }, w{ 0 }, h{0};
};

class Form {

public:
	Form(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#C1C1C1"));
	Form() {};
	virtual ~Form() {};

	bool hit_test(int mx, int my);
	virtual void render(Shader* s){};
	virtual void update(){};
	virtual void move(float _x, float _y);
	
	void resize();

	enum WidgetType;
	virtual WidgetType type() 
	{
		return _FORM;
	};
	enum WidgetType{
		_FORM, _HELPER,
		_MAIN_MENU,
		_DROP_DOWN,
		_TEXTURE_QUAD,
		_POP_UP_MENU
	};
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
	Form* popup_menu = nullptr;
public:
	void push_back(Form* g) { 
		gui_form_container.push_back(g); 
	};
	Form* pop_back(){
		Form* g = gui_form_container.back();
		gui_form_container.pop_back();
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
	
	const std::list<Form*>& get_container() { return gui_form_container; };
	
	~FormContainer() {
		for(auto& w : gui_form_container){
			delete w;
		}
		if (popup_menu) delete popup_menu;
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

class Popup;
class MainMenu : public Form
{
	std::vector<std::string> menu_items;
	std::vector<TextureQuad> icon_buttons;
	HighlightQuad* highlighter = nullptr;
	Popup* popup{nullptr};
	unsigned int b_ebo;
	struct { int index = 0; bool highlight = false; } highlight_info;
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

	void update();

	int test_item_hit(int mx, int my, quad* q);

	WidgetType type(){
		return _MAIN_MENU;
	}
private:
	void updatePopup(int index, quad& q);
};

class TextBox : public Form {
public:
	TextBox(int _x, int _y, unsigned int _w, unsigned int _h, Color _c = hexCodeToRGB("#FFFFCE"));
	TextBox() {};
	void include_text(std::string t){
		text = t;
	};

	void clear_text() {
		text.clear();
	};

	void render(Shader* s);

	WidgetType type(){
		return _HELPER;
	}
private:
	std::string text;
	unsigned int border_ebo;
};

class StaticTextMessage : public Form{
public:
	StaticTextMessage(std::string message, int _x, int _y, Color bkgrnd = Color::hex("#FFFFCE"));
	void render(Shader* s);
private:
	std::string message;
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
	Popup(std::string structure, int _x, int _y, unsigned int _w, unsigned int _h, Color _c = Color::hex("#D4D0C8"));
	~Popup();
	void render(Shader* s);

	WidgetType type(){
		return _POP_UP_MENU;
	}

	int max_item_string_size();;

	int test_item_hit(int my, quad* q, int* index);
	void highlight_item(const quad& q);
	void delete_highlighter();
	MenuPopupItem get_item(int id) const;
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

