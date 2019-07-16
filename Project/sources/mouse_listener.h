#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include "camera.h"

#define MOUSE_LFT GLFW_MOUSE_BUTTON_LEFT
#define MOUSE_RGT GLFW_MOUSE_BUTTON_RIGHT
#define MOUSE_MID GLFW_MOUSE_BUTTON_MIDDLE

struct MouseListener {

	//note to myself, validate state first
	enum State {
		NIL = 0x01,
		CLICK = 0x02,
		DRAG = 0x03,
	};

	enum CallBack {
		BUTTON_CALLBACK,
		MOVING_CALLBACK,
		CNIL
	};

	enum Agenda {
		ADD_NODE = 9,
		CONNECT_ELE = 8,
		SELECT_NODE = 7,
		RUN_ANALYSIS = 10,
	};

	struct Event {
		enum Flag {
			LFT_CLK = MOUSE_LFT | State::CLICK,
			RGT_CLK = MOUSE_RGT | State::CLICK,
			MID_CLK = MOUSE_MID | State::CLICK,
		};

		Flag flag;
		Flag prev_flag;

		bool left_click() {
			return flag == LFT_CLK && flag != prev_flag;
		}

		bool right_click() {
			return flag == RGT_CLK && flag != prev_flag;
		}

		bool mid_click() {
			return flag == MID_CLK && flag != prev_flag;
		}
	};

	State state = NIL;
	Agenda agenda = ADD_NODE;
	CallBack callback = CNIL;
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

	inline Event pack_event(Event::Flag prev) {
		Event ev;
		ev.flag = static_cast<Event::Flag>(button | state);
		ev.prev_flag = prev;
		return ev;
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
	if (mouse_listener.state == MouseListener::CLICK) {
		mouse_listener.state = MouseListener::DRAG;
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
	mouse_listener.callback = MouseListener::MOVING_CALLBACK;
}
