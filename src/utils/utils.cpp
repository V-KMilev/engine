#include "utils.h"

namespace Utils {
	namespace Time {
		float deltaTime = 0;
		int FPS         = 0;
		int frame       = 0;
	};

	namespace IO {
		Engine::InputManager inputManager;
	};

	namespace Render {
		Core::Renderer renderer;
	}
};
