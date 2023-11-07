#include "input_key.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine{
	State getState(GLFWwindow *window, int key, KeyType type) {
		if (type == KeyType::KEYBORD) {
			if(glfwGetKey(window, key) == GLFW_PRESS) {
				return State::PRESS;
			}
			else if(glfwGetKey(window, key) == GLFW_RELEASE) {
				return State::RELEASE;
			}
			else if (glfwGetKey(window, key) == GLFW_REPEAT) {
				return State::REPEAT;
			}
		}
		else if(type == KeyType::MOUSE) {
			if(glfwGetMouseButton(window, key) == GLFW_PRESS) {
				return State::PRESS;
			}
			else if(glfwGetMouseButton(window, key) == GLFW_RELEASE) {
				return State::RELEASE;
			}
		}
		return State::IDLE;
	}

	std::string stateToString(State state) {
		std::string state_str = "Invalid";

		if(state == State::PRESS) {
			state_str = "PRESS";
		}
		else if(state == State::RELEASE) {
			state_str = "RELEASE";
		}
		else if(state == State::REPEAT) {
			state_str = "REPEAT";
		}
		else if(state == State::IDLE) {
			state_str = "IDLE";
		}
		return state_str;
	}

	bool InputKey::operator == (const InputKey& other) const {
		return key == other.key && state == other.state;
	}
};
