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
};
