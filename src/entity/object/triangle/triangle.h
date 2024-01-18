#pragma once

#include "object.h"

namespace Engine {
	class Triangle : public Object {
		public:
			Triangle();

			void load_mesh() override;
	};
};
