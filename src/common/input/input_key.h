#pragma once

#include <functional>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Engine {
	enum class KeyType {
		NONE     = 0,
		KEYBOARD = 1,
		MOUSE    = 2
	};

	enum class State {
		IDLE    = 0,
		PRESS   = 1,
		RELEASE = 2,
		REPEAT  = 3
	};

	State getState(GLFWwindow *window, int key, KeyType type);

	std::string stateToString(State state);

	class InputKey {
		public:
			InputKey();
			InputKey(int key, State state);
			InputKey(int key, State state, KeyType type);

			bool operator == (const InputKey& other) const;

		private:
			void setType();

		public:
			int key;
			State state;
			KeyType type;
	};
};

// This is necessary to enable efficient lookups in an std::unordered_map that uses InputKey objects as keys.
// This ensures that the map works correctly with InputKey.
namespace std {
	template <>
	struct hash<Engine::InputKey> {

		std::size_t operator () (const Engine::InputKey& k) const {
			return std::hash<int>()(k.key) ^ std::hash<Engine::State>()(k.state) ^ std::hash<Engine::KeyType>()(k.type);
		}
	};
}
