#include "input_key.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine{
	State getState(GLFWwindow *window, int key, KeyType type) {
		if (type == KeyType::KEYBORD) {
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

	bool InputKey::operator == (const InputKey& other) const {
		return key == other.key && state == other.state;
	}
};
