#include "entity_id.h"

#include "entity.h"

namespace Engine {
	unsigned int EntityID::_mObjectID = 1;
	unsigned int EntityID::_mMeshID   = 1;
	unsigned int EntityID::_mCameraID = 1;
	unsigned int EntityID::_mGlobalID = 1;

	unsigned int EntityID::getID() const {
		return _mID;
	}

	unsigned int EntityID::getGlobalID() const {
		return _mGID;
	}

	void EntityID::updateID(EntityType type) {
		if (type == EntityType::OBJECT) {
			_mID = _mObjectID++;
		}
		else if(type == EntityType::MESH) {
			_mID = _mMeshID++;
		}
		else if(type == EntityType::CAMERA) {
			_mID = _mCameraID++;
		}
		else {
			return;
		}

		_mGID = _mGlobalID++;
	}
};
