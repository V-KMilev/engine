#include <iostream>

#include "window.h"

#include "tracer.h"

int main(void) {
	printf("Staring...\n");

	Profiler::Tracer tracer("tracer.json", "Engine Process", "Main Thread");

	const uint32_t width  = 1920;
	const uint32_t hight  = 1080;
	const uint32_t AA     = 8;
	const bool fullscreen = true;

	Engine::Window window("VKM Engine", width, hight, AA, fullscreen);

	if(!window.init()) {
		return -1;
	}

	window.main_loop();

	printf("Ending...\n");

	return 0;
}
