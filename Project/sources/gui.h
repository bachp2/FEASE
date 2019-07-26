#pragma once
#include "shader.h"
#include "text.h"
#include "tbuffer.h"
#include "config_parser.h"
#include "mouse_listener.h"
#include "key_listener.h"
#include "shader_manager.h"
#include <list>

struct Color;
class Printer;
class FormContainer;
class TextureQuad;

extern glm::mat4 per_proj, view, model, ort_proj;
extern ConfigParser configTable;
extern ShaderManager shaderTable;
extern MouseListener mouse_listener;
extern int scrWidth, scrHeight;
extern Printer* mPrinter;
extern FormContainer gui_container;

struct ViewPort {
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

	virtual void resize();

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

	~HighlightQuad();
	
	int getx() { return int(this->x); }
	int gety() { return int(this->y); }

	virtual void render(Shader* s);
	virtual void move(float _x, float _y){};

	float x, y;
protected:
	unsigned int vbo, vao, ebo[3];
	unsigned int width, height;
};

class Cursor : public HighlightQuad {
public:
	Cursor(int _x, int _y, unsigned int _w, unsigned int _h) : HighlightQuad(_x, _y,_w,_h) {
		this->style = Style::SOLID;
		this->x = _x;
		this->y = _y;
		this->width = _w;
		this->height = _h;
	}
	
	void move(float _x, float _y) {
		this->x = _x;
		this->y = _y;
	}

	void render(Shader* s) {
		//HighlightQuad::render(s);
		if (glfwGetTime() - blink_timer > 0.5) {
			visibility = !visibility;
			blink_timer = glfwGetTime();
		}
		if (!visibility) return;
		glDisable(GL_DEPTH_TEST);
		s->use();
		glm::mat4 _model = glm::mat4(1.0f);
		_model = glm::translate(_model, glm::vec3(x, y, 0));
		s->setMat4("model", _model);
		s->setMat4("projection", ort_proj);

		glBindVertexArray(vao);

		switch (style) {
		case Style::POP:
			s->setColor("color", Color::White());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
			glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
			s->setColor("color", Color::Black());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
			glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
			glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
			break;
		case Style::PRESSED:
			s->setColor("color", Color::Black());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
			glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
			s->setColor("color", Color::White());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
			glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
			glDrawElements(GL_TRIANGLES, 8 * 2, GL_UNSIGNED_INT, 0);
			break;
		case Style::SOLID:
			s->setColor("color", Color::hex("#1C76E3"));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			break;
		}

		glEnable(GL_DEPTH_TEST);
	}
	void make_cursor_visible() {
		this->blink_timer = glfwGetTime();
		this->visibility = true;
	}
	int ln{ 0 }, col{0};
private:
	double blink_timer{ 0 };
	bool visibility{true};
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

	void resize() {
		const float vertices[] = {
		0, 0, 0.0f,
		width, height, 0.0f,
		0, height, 0.0f,
		width, 0, 0.0f,
		width, height - 0.5f, 0.0f,
		0, height - 0.5f, 0.0f
		};

		const unsigned int indices[] = {
			0, 1, 2,
			0, 3, 1
		};

		const unsigned int border[] = { 4, 5 };

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

		glGenBuffers(1, &b_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border), border, GL_STATIC_DRAW);
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
	void move(float _x, float _y) {};

	~TextBox() {
		delete cursor;
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		glDeleteBuffers(1, &this->ebo);
		glDeleteBuffers(1, &this->border_ebo);
	}
private:
	TBuffer buf;
	unsigned int border_ebo;
	Cursor* cursor{nullptr};
	int hpad{2}, vpad{5};
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
		return mPrinter->get_font_height() * (items.size() + 1);
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

