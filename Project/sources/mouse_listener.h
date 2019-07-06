#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "camera.h"

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
	ADD_NODE = 9,
	CONNECT_ELE = 8,
	SELECT_NODE = 7,
	RUN_ANALYSIS = 10,
};

struct MouseListener {
	//note to myself, validate state first
	Mouse_State state = NIL;
	Mouse_Agenda agenda = ADD_NODE;
	Mouse_CallBack callback = CNIL;
	int button=-1;
	double cx{0}, cy{0}; //capture mouse's position from mouse button callback function
	double ox{0}, oy{0}; //capture mouse's vector from mouse callback function
	
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
	inline bool left_drag(double* dx, double* dy){
		auto r = draggedBy(GLFW_MOUSE_BUTTON_LEFT);
		if(r){
			*dx = cx-ox;
			*dy = cy-oy;
			ox = cx;
			oy = cy;
		}
		return r;
	}

	inline bool right_drag(double* dx, double* dy){
		auto r = draggedBy(GLFW_MOUSE_BUTTON_RIGHT);
		if(r){
			*dx = cx-ox;
			*dy = cy-oy;
			ox = cx;
			oy = cy;
		}
		return r;
	}

	inline bool middle_drag(){
		auto r = draggedBy(GLFW_MOUSE_BUTTON_MIDDLE);
		if(r){
			ox = cx;
			oy = cy;
		}
		return r;
	}

	inline bool left_click(){
		return clickedBy(GLFW_MOUSE_BUTTON_LEFT);
	}

	inline bool middle_click() { return clickedBy(GLFW_MOUSE_BUTTON_MIDDLE); }
	inline bool right_click() { return clickedBy(GLFW_MOUSE_BUTTON_RIGHT); }
	inline bool right_click_once() { 
		auto r = (button == GLFW_MOUSE_BUTTON_RIGHT && state == NIL);
		if(r) button = -1;
		return r; 
	}
	inline bool left_click_once() { 
		auto r = (button == GLFW_MOUSE_BUTTON_LEFT && state == NIL);
		if(r) button = -1;
		return r;
	}

	inline bool nilState() {
		return state == NIL;
	}
};

//---------------------------------------------------------------------------------------------
// MOUSE MOVEMENT CALLBACK FUNCTION
//---------------------------------------------------------------------------------------------
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
extern MouseListener mouse_listener;
extern ArcBallCamera camera;
static inline void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouse_listener.state == CLICK) {
		mouse_listener.state = DRAG;
		mouse_listener.ox = mouse_listener.cx;
		mouse_listener.oy = mouse_listener.cy;
	}

	if (mouse_listener.middle_drag())
	{
		float xoffset = xpos - mouse_listener.cx;
		float yoffset = mouse_listener.cy - ypos; // reversed since y-coordinates go from bottom to top
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	mouse_listener.cx = xpos;
	mouse_listener.cy = ypos;
	mouse_listener.callback = MOVING_CALLBACK;
}
