#pragma once
#include "common.h"
#include <stdio.h>
#include <ctime>

class KeyListener 
{
public:
	static KeyListener* Instance;
	static bool onPressed() {
		return Instance->action == GLFW_PRESS;
	}

	static bool readyForHoldEvent() {
		auto duration = (clock() - Instance->ref) / (double)CLOCKS_PER_SEC;
		const static auto thres = 0.3; // second
		if (duration <= thres) {
			return false;
		}
		return true;
	}

	static void parse_combo() {
		
	}

	int action = GLFW_RELEASE;
	unsigned int m_char{ 0 };
	clock_t ref{ 0 };
private:
	KeyListener() {};
};

inline static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_UNKNOWN) return; // Don't accept unknown keys
	if (action == GLFW_PRESS)
	{
		switch(key) {
		case GLFW_KEY_ENTER:
		case GLFW_KEY_KP_ENTER:
			KeyListener::Instance->m_char = '\n';
			break;
		case GLFW_KEY_BACKSPACE:
			KeyListener::Instance->m_char = GLFW_KEY_BACKSPACE;
			break;
		}
		KeyListener::Instance->ref = clock();
		KeyListener::Instance->action = action;
	}
	else if (action == GLFW_RELEASE)
	{
		KeyListener::Instance->m_char = 0;
		KeyListener::Instance->action = action;
	}
}

inline static void character_callback(GLFWwindow* window, unsigned int codepoint) {
	KeyListener::Instance->m_char = codepoint;
}
