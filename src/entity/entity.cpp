#include "entity.h"

namespace Engine {
	Entity::Entity(EntityType type) : _mType(type) {
		_mID.updateID(_mType);
	}

	unsigned int Entity::getID() const {
		return _mID.getID();
	}

	EntityType Entity::getType() const {
		return _mType;
	}

	const std::shared_ptr<Transform>& Entity::getTransform() const {
		return _mTransform;
	}

	std::shared_ptr<Transform>& Entity::getTransform() {
		return _mTransform;
	}

	const std::shared_ptr<InteractionState>& Entity::getInteractionState() const {
		return _mInteractionState;
	}

	std::shared_ptr<InteractionState>& Entity::getInteractionState() {
		return _mInteractionState;
	}

	json Entity::toJson() const {
		json entityJson;

		// Add entity properties to the JSON object
		entityJson["id"] = _mID.getID();
		entityJson["type"] = static_cast<int>(_mType);

		// Add transform properties
		if (_mTransform) {
			entityJson["transform"] = _mTransform->toJson();
		}

		// Add interaction state properties
		if (_mInteractionState) {
			entityJson["interactionState"] = _mInteractionState->toJson();
		}

		return entityJson;
	}
};
