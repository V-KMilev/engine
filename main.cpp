#include <iostream>

#include "window.h"

int main(void) {
	printf("Staring..\n");

	const uint32_t width  = 1920;
	const uint32_t hight  = 1080;
	const uint32_t AA     = 8;
	const bool fullscreen = true;

	Engine::Window window("VKM Engine", width, hight, AA, fullscreen);

	if(!window.init()) {
		return -1;
	}

	window.main_loop();

	printf("Ending..\n");

	return 0;
}
