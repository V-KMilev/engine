#include "entity.h"

namespace Engine {
	Entity::Entity(EntityType type) : _mType(type) {
		_mID.updateID(_mType);
	}

	unsigned int Entity::getID() const {
		return _mID.getID();
	}

	EntityType Entity::getTpye() const {
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
};
