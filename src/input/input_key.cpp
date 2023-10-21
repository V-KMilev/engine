#include "input_key.h"

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
		if(state == STATE::PRESS) {
			return "PRESS";
		}
		else if(state == STATE::RELEASE) {
			return "RELEASE";
		}
		else if(state == STATE::REPEAT) {
			return "REPEAT";
		}
		else if(state == STATE::IDLE) {
			return "Invalid State";
		}
	}

	bool InputKey::operator == (const InputKey& other) const {
		return key == other.key && state == other.state;
	}
};
