#pragma once

#include <unordered_set>
#include <vector>

#include "input_key.h"

namespace Engine {
	class KeyCombination {
		public:
			KeyCombination(const InputKey& inputKey);
			KeyCombination(const std::vector<InputKey>& inputKeys);

			bool operator == (const KeyCombination& other) const;

		public:
			std::unordered_set<InputKey> keys;
	};
}

// This is necessary to enable efficient lookups in an std::unordered_map that uses KeyCombination objects as keys.
// This ensures that the map works correctly with KeyCombination.
namespace std {
	template <>
	struct hash<Engine::KeyCombination> {

		std::size_t operator () (const Engine::KeyCombination& k) const {
			std::size_t hashValue = 0;

			for (const auto& inputKey : k.keys) {
				hashValue ^= std::hash<int>()(inputKey.key) ^ std::hash<Engine::State>()(inputKey.state);
			}

			return hashValue;
		}
	};
}