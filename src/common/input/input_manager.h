#pragma once

#include <unordered_map>
#include <functional>

#include "input_key.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {
	struct Mouse {
		bool isLeftPressed  = false;
		bool isRightPressed = false;

		int x = 0.0f;
		int y = 0.0f;

		float scrollX = 0.0f;
		float scrollY = 0.0f;

		double speed = 0.1f;

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

			void mapKeyandStatetoEvent(int key, State state, const EventCallback& event, const std::string& event_hint = "");

			void processMouse(GLFWwindow* window);

		public:
			static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
			static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			void processKey(GLFWwindow* window, int key, KeyType tpye) const;

		private:
			Mouse _mMouse;

			std::unordered_map<InputKey, EventCallback> _mMapOfEvents;
	};
};
