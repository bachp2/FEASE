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
	double _cx{0}, _cy{0}; //capture mouse's position from mouse button callback function
	double _dx{0}, _dy{0}; //capture mouse's vector from mouse callback function
	
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
	inline bool left_drag(){
		auto r = draggedBy(GLFW_MOUSE_BUTTON_LEFT);
		if(r){
			_dx = _cx;
			_dy = _cy;
		}
		return r;
	}

	inline bool right_drag(){
		auto r = draggedBy(GLFW_MOUSE_BUTTON_RIGHT);
		if(r){
			_dx = _cx;
			_dy = _cy;
		}
		return r;
	}

	inline bool middle_drag(){
		auto r = draggedBy(GLFW_MOUSE_BUTTON_MIDDLE);
		if(r){
			_dx = _cx;
			_dy = _cy;
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
	
	inline void resetState() { 
		state = NIL; 
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
	static float lastX = xpos;
	static float lastY = ypos;

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	mouse_listener._cx = xpos;
	mouse_listener._cy = ypos;
	/*mouse_listener._dx = xoffset;
	mouse_listener._dy = yoffset;*/
	if (mouse_listener.state == CLICK) {
		mouse_listener.state = DRAG;
	}

	lastX = xpos;
	lastY = ypos;

	if (mouse_listener.middle_drag())
		camera.ProcessMouseMovement(xoffset, yoffset);

	// hit detection when outside of any active widgets
	//gui_widget_container.generic_hit_testing_widgets();

	/*for(auto& w : gui_widget_container){
	if(w->moveable && mouse_event_listener.draggedBy(GLFW_MOUSE_BUTTON_LEFT)){
	w->move(xoffset, -yoffset);
	}
	}*/
	//printf("%.2f %.2f\n", xoffset, yoffset);
	mouse_listener.callback = MOVING_CALLBACK;
}