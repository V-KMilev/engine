#pragma once

#include <unordered_map>
#include <functional>

#include "input_key.h"
#include "input_key_combination.h"

struct GLFWwindow;

namespace Engine {
	struct Mouse {
		int x = 0.0f;
		int y = 0.0f;

		float scrollX = 0.0f;
		float scrollY = 0.0f;

		float speed = 0.1f;

		bool hasUpdate = false;
	};

	using EventCallback = std::function<void()>;

	class InputManager {
		public:
			InputManager();
			~InputManager() = default;

			InputManager(const InputManager& other) = delete;
			InputManager& operator =(const InputManager& other) = delete;

			InputManager(InputManager && other) = delete;
			InputManager& operator =(InputManager && other) = delete;

			const Mouse& getMouse() const;
			Mouse& getMouse();

			void mapInputKeyToEvent(const InputKey& key, const EventCallback& event, const std::string& event_hint = "");
			void mapKeyCombinationToEvent(const KeyCombination& keyCombinations, const EventCallback& event, const std::string& event_hint = "");

			void processMouse(GLFWwindow* window);

		public:
			static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
			static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			void processKey(GLFWwindow* window, int key, KeyType type) const;

		private:
			Mouse _mMouse;

			std::unordered_map<KeyCombination, EventCallback> _mMapOfEvents;
	};
};
