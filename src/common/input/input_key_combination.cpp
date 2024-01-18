#include "input_key_combination.h"

namespace Engine {
	KeyCombination::KeyCombination(const InputKey& inputKey) {
		keys.insert(inputKey);
	}

	KeyCombination::KeyCombination(const std::vector<InputKey>& inputKeys) {
		for (const auto& inputKey : inputKeys) {
			keys.insert(inputKey);
		}
	}

	bool KeyCombination::operator == (const KeyCombination& other) const {
		return keys == other.keys;
	}
}