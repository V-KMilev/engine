#pragma once

#include "object.h"

namespace Engine {
	class Quad : public Object {
		public:
			Quad();

			void load_mesh() override;
	};
};
