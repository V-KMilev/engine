#include "input_key.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine{
	State getState(GLFWwindow *window, int key, KeyType type) {
		if (type == KeyType::KEYBOARD) {
			if (glfwGetKey(window, key) == GLFW_PRESS) {
				return State::PRESS;
			}
			else if (glfwGetKey(window, key) == GLFW_RELEASE) {
				return State::RELEASE;
			}
			else if (glfwGetKey(window, key) == GLFW_REPEAT) {
				return State::REPEAT;
			}
		}
		else if (type == KeyType::MOUSE) {
			if (glfwGetMouseButton(window, key) == GLFW_PRESS) {
				return State::PRESS;
			}
			else if (glfwGetMouseButton(window, key) == GLFW_RELEASE) {
				return State::RELEASE;
			}
		}
		return State::IDLE;
	}

	std::string stateToString(State state) {
		switch (state) {
			case State::PRESS:   return "PRESS";
			case State::RELEASE: return "RELEASE";
			case State::REPEAT:  return "REPEAT";
			case State::IDLE:    return "IDLE";
			default:             return "INVALID";
		}
	}

	InputKey::InputKey() {
		setType();
	}

	InputKey::InputKey(int key, State state) : key(key), state(state) {
		setType();
	}

	InputKey::InputKey(int key, State state, KeyType type) : key(key), state(state), type(type) {}

	bool InputKey::operator == (const InputKey& other) const {
		return key == other.key && state == other.state && type == other.type;
	}

	void InputKey::setType() {
		if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_LAST) {
			type = KeyType::KEYBOARD;
		} else if (key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_LAST) {
			type = KeyType::MOUSE;
		} else {
			type = KeyType::NONE;
		}
	}
};
