#pragma once
#include <glad/glad.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <atomic>
enum Mouse_State {
	NIL,
	CLICK,
	DRAG,
	LIMBO
};

enum Mouse_CallBack {
	BUTTON_CALLBACK,
	MOVING_CALLBACK,
	CNIL
};

enum Mouse_Agenda {
	ADD_NODE,
	CONNECT_ELE,
	SELECT_NODE,
	RUN_ANALYSIS,
};

struct MouseListener {
	//note to myself, validate state first
	Mouse_State state = NIL;
	Mouse_Agenda agenda = ADD_NODE;
	Mouse_CallBack callback = CNIL;
	int button=0;
	double _cx, _cy; //capture mouse's position from mouse button callback function
	double _dx, _dy; //capture mouse's vector from mouse callback function
	inline bool draggedBy(int btn) {
		if (button == btn && state == DRAG) return true;
		return false;
	}

	inline bool clickedBy(int btn) {
		if (button == btn && state == CLICK) {
			return true;
		}
		return false;
	}

	inline bool left_click(){
		return clickedBy(GLFW_MOUSE_BUTTON_LEFT);
	}

	inline bool middle_click() { return clickedBy(GLFW_MOUSE_BUTTON_MIDDLE); }
	inline bool right_click() { return clickedBy(GLFW_MOUSE_BUTTON_RIGHT); }
	inline bool right_click_once() { 
		auto r = (button == GLFW_MOUSE_BUTTON_RIGHT && state == NIL);
		if(r) {
			button = 0;
		}
		return r; 
	}
	inline bool left_click_once() { return button == GLFW_MOUSE_BUTTON_LEFT && state == NIL; }

	inline bool nilState() {
		return state == NIL;
	}
	
	inline void resetState() { 
		state = NIL; 
	}
};
