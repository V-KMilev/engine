#pragma once

#include <unordered_map>
#include <functional>

#include "input_key.h"

// To avoid including the full GLFW implementation, we forward declaration GLFWwindow
class GLFWwindow;

namespace Engine {
	struct Mouse {
		bool isLeftPressed  = false;
		bool isRightPressed = false;

		double x = 0.0f;
		double y = 0.0f;

		double speed = 0.1f;
	};

	class InputHandle {
		public:
			InputHandle();
			~InputHandle() = default;

			InputHandle(const InputHandle& other) = delete;
			InputHandle& operator =(const InputHandle& other) = delete;

			InputHandle(InputHandle && other) = delete;
			InputHandle& operator =(InputHandle && other) = delete;

			const Mouse& getMouse() const;
			Mouse& getMouse();

			void mapKeyandStatetoEvent(int key, State state, std::function<void()> event, const std::string& event_hint = "");

			void processPos(GLFWwindow* window);
		public:
			static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			void processKey(GLFWwindow* window, int key, KeyType tpye) const;

		private:
			Mouse _mMouse;

			std::unordered_map<InputKey, std::function<void()>> _mapOfEvents;
	};
};
