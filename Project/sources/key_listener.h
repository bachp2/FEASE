#pragma once
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <ctime>
#include <string>

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

	static int parse_key_to_char(int c, int mods) {

		switch (mods)
		{
		case GLFW_MOD_SHIFT:
			if (c >= 'A' && c <= 'Z') {
				return c;
			}
			if (c >= '1' && c <= '9') {
				return '!' + c - '1';
			}
		}

		if (c >= 'A' && c <= 'Z') {
			c += 'a' - 'A';
			return c;
		}

		if (c == GLFW_KEY_ENTER) return '\n';
		return -1;
	}

	int action = GLFW_RELEASE;
	int m_char{ 0 };
	clock_t ref{ 0 };
private:
	KeyListener() {};
};

inline static void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_UNKNOWN) return; // Don't accept unknown keys

	if (action == GLFW_PRESS)
	{
		KeyListener::Instance->ref = clock();
		KeyListener::Instance->m_char = KeyListener::parse_key_to_char(key, mods);
		KeyListener::Instance->action = action;
	}
	else if (action == GLFW_RELEASE)
	{
		KeyListener::Instance->m_char = 0;
		KeyListener::Instance->action = action;
	}
}
