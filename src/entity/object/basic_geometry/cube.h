#pragma once

#include "object.h"

namespace Engine {
	class Cube : public Object {
		public:
			Cube();

			void load_mesh() override;
	};
};
