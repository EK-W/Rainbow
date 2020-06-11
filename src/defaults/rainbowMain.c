#include "headers/RB_Main.h"
#include "headers/RB_AssignmentQueue.h"
#include "headers/RB_BasicTypes.h"
#include "headers/RB_ColorPool.h"
#include "headers/RB_PixelMap.h"
#include "headers/RB_Display.h"
#include <stdio.h>
#include <stdlib.h>


void setPixelColor(RB_Data* data, RB_Coord coord, RB_Color color) {
	RB_Pixel* toSet = RB_getPixel(data->pixelMap, coord);

	if(toSet->status == RB_PIXEL_SET) {
		fprintf(
			stderr,
			"Attempting to set Pixel at (%d,%d) even though it is already set!\n"
			"\tQueue size: %d\n",
			toSet->loc.x,
			toSet->loc.y,
			RB_getQueueSize(data->assignmentQueue)
		);
		return;
	}
	if(RB_coordIsInQueue(data->assignmentQueue, toSet->loc)) {
		RB_removeCoordFromAssignmentQueue(data->assignmentQueue, toSet->loc);	
	}
	RB_removeColorFromPool(data->colorPool, color);

	toSet->color = color;
	toSet->status = RB_PIXEL_SET;

	RB_setDisplayedPixelColor(data->display, toSet->loc, color);

	RB_addResultantCoordsToQueue(data->pixelMap, data->assignmentQueue, toSet->loc);
}

RB_Data* RB_init(RB_Config config) {
	if(config.width * config.height != config.rRes * config.gRes * config.bRes) {
		fprintf(stderr,
			"Error initializing rainbow! width * height must be equal to rRes * gRes * bRes\n"
			"width * height == %d * %d == %d\n"
			"rRes * gRes * bRes == %d * %d * %d == %d\n",
			config.width, config.height, config.width * config.height,
			config.rRes, config.gRes, config.bRes, config.rRes * config.gRes * config.bRes
		);
		return NULL;
	}

	RB_Data* ret = (RB_Data*) malloc(sizeof(RB_Data));

	if(ret == NULL) {
		fprintf(stderr, "Failed to initialize Rainbow Data!\n");
		return NULL;
	}

	ret->assignmentQueue = NULL;
	ret->colorPool = NULL;
	ret->pixelMap = NULL;
	ret->display = NULL;

	RB_Size numPixels = config.height * config.width;
	
	ret->assignmentQueue = RB_createAssignmentQueue(numPixels, config.width, config.height);

	if(ret->assignmentQueue == NULL) {
		fprintf(stderr, "Failed to initialize Assignment Queue!\n");
		RB_free(ret);
		return NULL;
	}

	ret->colorPool = RB_createColorPool(config.rRes, config.gRes, config.bRes);

	if(ret->colorPool == NULL) {
		fprintf(stderr, "Failed to initialize Color Pool!\n");
		RB_free(ret);
		return NULL;
	}

	ret->pixelMap = RB_createPixelMap(config.width, config.height);

	if(ret->pixelMap == NULL) {
		fprintf(stderr, "Failed to initialize Pixel Map!\n");
		RB_free(ret);
		return NULL;
	}

	ret->display = RB_createDisplay(
		config.windowWidth, config.windowHeight,
		config.width, config.height,
		config.rRes, config.gRes, config.bRes
	);

	if(ret->display == NULL) {
		fprintf(stderr, "Failed to initialize Display!\n");
		RB_free(ret);
		return NULL;
	}

	// Set starting points
	setPixelColor(
		ret,
		(RB_Coord) { .x = config.width / 2, .y = config.height / 2 },
		(RB_Color) { .r = config.rRes / 2, .g = config.gRes / 2, .b = config.bRes / 2 }
	);

	return ret;
}

void RB_free(RB_Data* data) {
	if(data != NULL) {
		printf("Freeing RB_Data!\n");
		RB_freeAssignmentQueue(data->assignmentQueue);
		RB_freeColorPool(data->colorPool);
		RB_freePixelMap(data->pixelMap);
		RB_freeDisplay(data->display);
		free(data);
	}
}



bool RB_generateNextPixel(RB_Data* data) {
	if(RB_isQueueEmpty(data->assignmentQueue)) {
		return false;
	}

	RB_Coord nextCoord = RB_chooseCoordFromAssignmentQueue(data->assignmentQueue);
	RB_Color preferredColor = RB_determinePreferredCoordColor(data->pixelMap, nextCoord);
	RB_Color idealColor = RB_findIdealAvailableColor(data->colorPool, preferredColor);

	setPixelColor(data, nextCoord, idealColor);

	return !RB_isQueueEmpty(data->assignmentQueue);
}