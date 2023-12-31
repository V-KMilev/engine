#include <iostream>

#include "window.h"

int main(void) {
	printf("Staring..\n");

	const uint32_t width  = 800;
	const uint32_t hight  = 400;
	const uint32_t AA     = 4;
	const bool fullscreen = false;

	Engine::Window window("VKM Engine", width, hight, AA, fullscreen);

	if(!window.init()) {
		return -1;
	}

	window.main_loop();

	printf("Ending..\n");

	return 0;
}