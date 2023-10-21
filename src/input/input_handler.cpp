#include "input_handler.h"

#include "logger.h"

namespace Engine{
	InputHandle::InputHandle() : _mapOfEvents({}) {}

	void InputHandle::processInput(GLFWwindow* window, int key) const {
		STATE state = getState(window, key);

		// Check if an event is mapped for this key and state.
		auto it = _mapOfEvents.find({key, state});

		// If an event is found, execute it.
		if (it != _mapOfEvents.end()) {
			it->second();
		}
	}

	void InputHandle::mapKeyandStatetoEvent(int key, STATE state, std::function<void()> event, const std::string& event_hint) {
		if (state == STATE::IDLE) {
			LOG("Mapping to `IDLE` state is not allowed", LOG_LEVEL::L_WARN);
			return;
		}

		LOG("Mapped Key: %d & State: %s to %s", LOG_LEVEL::L_INFO, key, stateToString(state).c_str(), event_hint.c_str());

		// Store the provided event in the map based on key and state.
		_mapOfEvents[{key, state}] = event;
	}

	void InputHandle::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// You can retrieve the user pointer to get the InputHandle instance
		InputHandle* inputHandle = static_cast<InputHandle*>(glfwGetWindowUserPointer(window));

		if (inputHandle) {
			inputHandle->processInput(window, key);
		}
	}
};
