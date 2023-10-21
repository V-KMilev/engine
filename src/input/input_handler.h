#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <functional>

#include "input_key.h"

namespace Engine {
	class InputHandle {
		public:
			InputHandle();
			~InputHandle() = default;

			InputHandle(const InputHandle& other) = delete;
			InputHandle& operator =(const InputHandle& other) = delete;

			InputHandle(InputHandle && other) = delete;
			InputHandle& operator =(InputHandle && other) = delete;

			bool init();
			bool deinit();

			void mapKeyandStatetoEvent(int key, STATE state, std::function<void()> event, const std::string& event_hint = "");

		public:
			static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			void processInput(GLFWwindow* window, int key) const;

		private:
			std::unordered_map<InputKey, std::function<void()>> _mapOfEvents;
	};
};
