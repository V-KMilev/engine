#include "input_manager.h"

#include "logger.h"

namespace Engine {
	InputManager::InputManager() : _mMapOfEvents({}) {}

	const Mouse& InputManager::getMouse() const {
		return _mMouse;
	}

	Mouse& InputManager::getMouse() {
		return _mMouse;
	}

	void InputManager::mapKeyandStatetoEvent(int key, State state, const EventCallback& event, const std::string& event_hint) {
		if (state == State::IDLE) {
			LOG("Mapping '%c' to '%s' state is not allowed", LOG_LEVEL::L_WARN, key, stateToString(state).c_str());
			return;
		}

		if (key > 31 && key < 97) {
			LOG("Mapped Key/State: '%c' & '%s' to '%s'", LOG_LEVEL::L_INFO, key, stateToString(state).c_str(), event_hint.c_str());
		}
		else {
			LOG("Mapped Key/State: '%d' & '%s' to '%s'", LOG_LEVEL::L_INFO, key, stateToString(state).c_str(), event_hint.c_str());
		}

		// Store the provided event in the map based on key and state.
		_mMapOfEvents[{key, state}] = event;
	}

	// TODO: Think of way to process inputs parallel

	void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// You can retrieve the user pointer to get the InputManager instance
		InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

		if (inputManager) {
			inputManager->processKey(window, key, KeyType::KEYBORD);
		}
	}

	void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		// You can retrieve the user pointer to get the InputManager instance
		InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

		if (inputManager) {
			inputManager->processKey(window, button, KeyType::MOUSE);
		}
	}

	void InputManager::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		// You can retrieve the user pointer to get the InputManager instance
		InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));

		if (inputManager) {
			inputManager->getMouse().scrollX = xoffset;
			inputManager->getMouse().scrollY = yoffset;
			inputManager->getMouse().hasUpdate = true;
		}
	}

	void InputManager::processKey(GLFWwindow* window, int key, KeyType tpye) const {
		State state = getState(window, key, tpye);

		// Check if an event is mapped for this key and state.
		auto it = _mMapOfEvents.find({key, state});

		// If an event is found, execute it.
		if (it != _mMapOfEvents.end()) {
			it->second();
		}
	}

	void InputManager::processMouse(GLFWwindow* window) {
		double x = 0.0f;
		double y = 0.0f;
		glfwGetCursorPos(window, &x, &y);
		_mMouse.x = x;
		_mMouse.y = y;
	}
};
