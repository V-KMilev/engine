#include "input_handler.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "logger.h"

namespace Engine{
	InputHandle::InputHandle() : _mapOfEvents({}) {}

	const Mouse& InputHandle::getMouse() const {
		return _mMouse;
	}

	Mouse& InputHandle::getMouse() {
		return _mMouse;
	}

	void InputHandle::mapKeyandStatetoEvent(int key, State state, std::function<void()> event, const std::string& event_hint) {
		if (state == State::IDLE) {
			LOG("Mapping '%c' to '%s' state is not allowed", LOG_LEVEL::L_WARN, key, stateToString(state).c_str());
			return;
		}

		LOG("Mapped Key/State: '%c' & '%s' to '%s'", LOG_LEVEL::L_INFO, key, stateToString(state).c_str(), event_hint.c_str());

		// Store the provided event in the map based on key and state.
		_mapOfEvents[{key, state}] = event;
	}

	void InputHandle::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// You can retrieve the user pointer to get the InputHandle instance
		InputHandle* inputHandle = static_cast<InputHandle*>(glfwGetWindowUserPointer(window));

		if (inputHandle) {
			inputHandle->processKey(window, key, KeyType::KEYBORD);
		}
	}

	void InputHandle::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		// You can retrieve the user pointer to get the InputHandle instance
		InputHandle* inputHandle = static_cast<InputHandle*>(glfwGetWindowUserPointer(window));

		if (inputHandle) {
			inputHandle->processKey(window, button, KeyType::MOUSE);
		}
	}

	void InputHandle::processKey(GLFWwindow* window, int key, KeyType tpye) const {
		State state = getState(window, key, tpye);

		// Check if an event is mapped for this key and state.
		auto it = _mapOfEvents.find({key, state});

		// If an event is found, execute it.
		if (it != _mapOfEvents.end()) {
			it->second();
		}
	}

	void InputHandle::processPos(GLFWwindow* window) {
		glfwGetCursorPos(window, &_mMouse.x, &_mMouse.y);
	}
};
