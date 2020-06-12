#include "headers/RB_Main.h"
#include "headers/RB_Display.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	RB_Config* config = RB_newConfig();
	RB_setColorResolution(config, 64, 64, 64);
	RB_setWindowDimensions(config, 512, 512);

	RB_Data* rainbow = RB_init(config);

	RB_setCoordColor(rainbow, RB_getRandomCoord(rainbow), RB_getRandomColor(rainbow));

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
	RB_freeConfig(config);

	return 0;
}