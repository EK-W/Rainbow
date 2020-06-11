#include "headers/RB_Main.h"
#include "headers/RB_Display.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	srand(time(NULL));
	
	RB_Config config = {
		.width = 512,
		.height = 512,
		.rRes = 64,
		.gRes = 64,
		.bRes = 64,
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