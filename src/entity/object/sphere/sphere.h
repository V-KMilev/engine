#pragma once

#include "object.h"

namespace Engine {
	class Sphere : public Object {
		public:
			Sphere();

			void load_mesh() override;
	};
};
