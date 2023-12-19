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

	void InputManager::mapInputKeyToEvent(const InputKey& inputKey, const EventCallback& event, const std::string& event_hint) {
		if (inputKey.state == State::IDLE) {
			LOG("Mapping '%c' to '%s' state is not allowed", LOG_LEVEL::L_WARN, inputKey.key, stateToString(inputKey.state).c_str());
			return;
		}

		if (inputKey.key > 31 && inputKey.key < 97) {
			LOG("Mapped Key/State: '%c' & '%s' to '%s'", LOG_LEVEL::L_INFO, inputKey.key, stateToString(inputKey.state).c_str(), event_hint.c_str());
		}
		else {
			LOG("Mapped Key/State: '%d' & '%s' to '%s'", LOG_LEVEL::L_INFO, inputKey.key, stateToString(inputKey.state).c_str(), event_hint.c_str());
		}


		// Store the provided event in the map based on key and state.
		_mMapOfEvents[{inputKey}] = event;
	}

	void InputManager::mapKeyCombinationToEvent(const KeyCombination& keyCombinations, const EventCallback& event, const std::string& event_hint) {
		for (const auto& inputKey : keyCombinations.keys) {
			if (inputKey.state == State::IDLE) {
				LOG("Mapping '%c' to '%s' state is not allowed", LOG_LEVEL::L_WARN, inputKey.key, stateToString(inputKey.state).c_str());
				return;
			}
		}

		LOG("Mapped Key Combination:", LOG_LEVEL::L_INFO);

		for(const auto& inputKey : keyCombinations.keys) {
			if (inputKey.key > 31 && inputKey.key < 97) {
				LOG("\tKey/State: '%c' & '%s'", LOG_LEVEL::L_INFO, inputKey.key, stateToString(inputKey.state).c_str());
			}
			else {
				LOG("\tKey/State: '%d' & '%s'", LOG_LEVEL::L_INFO, inputKey.key, stateToString(inputKey.state).c_str());
			}
		}

		LOG("to '%s'", LOG_LEVEL::L_INFO, event_hint.c_str());

		// Store the provided event in the map based on the key combination.
		_mMapOfEvents[keyCombinations] = event;
	}

	void InputManager::processKey(GLFWwindow* window, int key, KeyType type) const {
		State state = getState(window, key, type);

		// Process individual keys
		InputKey individualKey{ key, state };

		// Check for key combinations
		for (const auto& [combination, event] : _mMapOfEvents) {
			if (combination.keys.find(individualKey) == combination.keys.end()) {
				// The individual key is not part of this combination
				continue;
			}

			// Check if all keys in the combination are in the desired state
			bool allKeysAreReady = true;

			for (const auto& keyInCombination : combination.keys) {
				if (getState(window, keyInCombination.key, type) != keyInCombination.state) {
					allKeysAreReady = false;
					break;
				}
			}

			if (allKeysAreReady) {
				event();
			}
		}
	}

	void InputManager::processMouse(GLFWwindow* window) {
		double x = 0.0f;
		double y = 0.0f;
		glfwGetCursorPos(window, &x, &y);
		_mMouse.x = x;
		_mMouse.y = y;
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
};
