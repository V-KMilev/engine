#include "input_key.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine{
	STATE getState(GLFWwindow *window, int key) {
		if(glfwGetKey(window, key) == GLFW_PRESS) {
			return STATE::PRESS;
		}
		else if(glfwGetKey(window, key) == GLFW_RELEASE) {
			return STATE::RELEASE;
		}
		else if (glfwGetKey(window, key) == GLFW_REPEAT) {
			return STATE::REPEAT;
		}
		return STATE::IDLE;
	}

	std::string stateToString(STATE state) {
		std::string state_str = "Invalid";

		if(state == STATE::PRESS) {
			state_str = "PRESS";
		}
		else if(state == STATE::RELEASE) {
			state_str = "RELEASE";
		}
		else if(state == STATE::REPEAT) {
			state_str = "REPEAT";
		}
		else if(state == STATE::IDLE) {
			state_str = "IDLE";
		}
		return state_str;
	}

	bool InputKey::operator == (const InputKey& other) const {
		return key == other.key && state == other.state;
	}
};
