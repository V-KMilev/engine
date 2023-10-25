#pragma once

#include <functional>
#include <string>

// To avoid including the full GLFW implementation, we forward declaration GLFWwindow
class GLFWwindow;

namespace Engine {
	enum class STATE {
		IDLE    = 0,
		PRESS   = 1,
		RELEASE = 2,
		REPEAT  = 3
	};

	STATE getState(GLFWwindow *window, int key);

	std::string stateToString(STATE state);

	class InputKey {
		public:
			bool operator == (const InputKey& other) const;

		public:
			int key;
			STATE state;
	};
};

// This is necessary to enable efficient lookups in an std::unordered_map that uses InputKey objects as keys.
// This ensures that the map works correctly with InputKey.
namespace std {
	template <>
	struct hash<Engine::InputKey> {
		std::size_t operator()(const Engine::InputKey& k) const {
			return std::hash<int>()(k.key) ^ std::hash<Engine::STATE>()(k.state);
		}
	};
}
