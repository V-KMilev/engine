#include "entity_id.h"

#include <utility>

namespace Core {
	unsigned int EntityID::_mIDCount = 1;

	EntityID::EntityID() : _mID(_mIDCount) {
		_mIDCount++;
	}

	EntityID::EntityID(const EntityID& other) {
		this->_mID = other._mID;
	}

	EntityID& EntityID::operator = (const EntityID& other) {
		if (this == &other) {
			return *this;
		}

		this->_mID = other._mID;
		return *this;
	}

	EntityID::EntityID(EntityID&& other) {
		this->_mID = std::move(other._mID);
	}

	EntityID& EntityID::operator = (EntityID&& other) {
		if (this == &other) {
			return *this;
		}

		this->_mID = std::move(other._mID);
		return *this;
	}

	unsigned int EntityID::getID() const {
		return _mID;
	}
};
