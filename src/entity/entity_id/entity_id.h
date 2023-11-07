#pragma once

namespace Core {
	class EntityID {
		public:
			EntityID();
			~EntityID() = default;

			EntityID(const EntityID& other);
			EntityID& operator = (const EntityID& other);

			EntityID(EntityID && other);
			EntityID& operator = (EntityID && other);

			unsigned int getID() const;

		private:
			unsigned int _mID;

			static unsigned int _mIDCount;
		};
};

