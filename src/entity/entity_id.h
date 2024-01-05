#pragma once

namespace Engine {
	enum class EntityType;
}

namespace Engine {
	class EntityID {
		public:
			EntityID() = default;
			~EntityID() = default;

			EntityID(const EntityID& other) = delete;
			EntityID& operator = (const EntityID& other) = delete;

			EntityID(EntityID && other) = delete;
			EntityID& operator = (EntityID && other) = delete;

			unsigned int getID() const;

			unsigned int getGlobalID() const;

			void updateID(EntityType type);

		private:
			unsigned int _mID = 0;
			unsigned int _mGID = 0;

			static unsigned int _mMeshID;
			static unsigned int _mObjectID;
			static unsigned int _mCameraID;
			static unsigned int _mLightID;
			static unsigned int _mGlobalID;
	};
};
