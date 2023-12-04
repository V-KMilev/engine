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

	const std::shared_ptr<WorldData>& Entity::getWorldData() const {
		return _mWorldData;
	}

	std::shared_ptr<WorldData>& Entity::getWorldData() {
		return _mWorldData;
	}

	const std::shared_ptr<UseData>& Entity::getUseData() const {
		return _mUseData;
	}

	std::shared_ptr<UseData>& Entity::getUseData() {
		return _mUseData;
	}
};
