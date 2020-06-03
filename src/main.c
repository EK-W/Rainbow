#include "headers/RB_Main.h"
#include "headers/RB_Display.h"
#include <stdbool.h>

int main(int argc, char** argv) {
	RB_Config config = {
		.width = 64,
		.height = 64,
		.rRes = 16,
		.gRes = 16,
		.bRes = 16,
		.windowWidth = 512,
		.windowHeight = 512
	};

	RB_Data* rainbow = RB_init(config);

	bool shouldQuit = false;

	while(RB_generateNextPixel(rainbow)) {
		RB_updateDisplay(rainbow->display);
		if(RB_handleWindowEvents(rainbow->display) == 0) {
			shouldQuit = true;
			break;
		}
	}

	if(!shouldQuit) {
		RB_forceUpdateDisplay(rainbow->display, false);
		while(RB_handleWindowEvents(rainbow->display) != 0);
	}
	

	RB_free(rainbow);

	return 0;
}