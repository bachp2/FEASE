#pragma once
#include <glad/glad.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

enum Mouse_State {
	NIL,//means the same as neutral but I'm too lazy to type such a long word!
	CLICK,
	DRAG,
	LIMBO //between Heaven and HEll !!
};

enum Mouse_Agenda {
	ADD_NODE,
	CONNECT_ELE,
	SELECT_NODE
};

struct MouseListener {
	//note to myself, validate state first
	Mouse_State state = NIL;
	Mouse_Agenda agenda = ADD_NODE;
	int button;
	bool flag = false;

	inline bool draggedBy(int btn) {
		if (button == btn && state == DRAG) return true;
		return false;
	}

	inline bool clickedBy(int btn) {
		if (button == btn && state == CLICK) return true;
		return false;
	}
	inline void resetState() { state = NIL; }
} mouseListener;
