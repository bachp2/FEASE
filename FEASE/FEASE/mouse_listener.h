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
struct MouseListener {
	//note to myself, validate state first
	Mouse_State state = NIL;
	int button;
	bool flag = false;
} mouseListener;
