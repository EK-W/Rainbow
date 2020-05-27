#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "rainbowImageGen.h"
#include "display.h"


typedef enum {
	PIXEL_STATUS_UNTOUCHED = 0,
	PIXEL_STATUS_QUEUED,
	PIXEL_STATUS_SET
} PointStatus;

typedef struct {
	Color col;
	PointStatus status;
} Pixel;

typedef struct {
	Pixel** map;
	crd_cmp width;
	crd_cmp height;
} PixelMap;

typedef struct {
	Coord* coords;
	size_t len;
} CoordQueue;

// typedef struct {
// 	uint_fast8_t*** colors;
// 	size_t r_len;
// 	size_t g_len;
// 	size_t b_len;
// } ColorSpace;

typedef struct {
	Color* list;
	size_t len;
} ColorList;


typedef struct {
	color_size_type r;
	color_size_type g;
	color_size_type b;
} ColorResolution;

// Removes a coordinate from the queue and returns it.
Coord chooseNextPixelCoord(CoordQueue* queue) {
	if(queue->len > RAND_MAX) {
		// TODO: problems.
	}
	if(queue->len == 0) {
		// TODO: problems
	}

	// This might be buggy if size_t is signed.
	size_t nextIndex = ((size_t) rand()) % queue->len;

	Coord nextCoord = queue->coords[nextIndex];
	

	// the no-shuffling option
	// for(rb_max_size_type i = nextIndex; i < queue->len - 1; i++) {
	// 	queue->coords[i] = queue->coords[i + 1];
	// }
	
	queue->coords[nextIndex] = queue->coords[queue->len - 1];
	queue->len--;

	return nextCoord;
}

bool coordIsWithinBounds(Coord loc, crd_cmp width, crd_cmp height) {
	return (
		(loc.x >= 0) && (loc.x < width)
		&& (loc.y >= 0) && (loc.y < height)
	);
}

Pixel* getPixelAtCoord(Coord loc, PixelMap pMap) {
	if(coordIsWithinBounds(loc, pMap.width, pMap.height)) {
		return &(pMap.map[loc.x][loc.y]);
	} else {
		return NULL;
	}
}



Color getPreferredColor(Coord loc, PixelMap pMap) {
	// TODO: find an abstracted way to represent "the data type that is at least two bits bigger than a color component"
	uint_fast16_t sumR = 0;
	uint_fast16_t sumG = 0;
	uint_fast16_t sumB = 0;
	uint_fast8_t numValidNeighbors = 0;
	
	for(int_fast8_t dx = -1; dx <= 1; dx++) {
		for(int_fast8_t dy = -1; dy <= 1; dy++) {
			if(dx == 0 && dy == 0) {
				continue;
			}
			Coord newCoord = { .x = loc.x + dx, .y = loc.y + dy };
			Pixel* p_raw = getPixelAtCoord(newCoord, pMap);

			if(p_raw == NULL) {
				continue;
			}
			Pixel p = *p_raw;
			if(p.status == PIXEL_STATUS_SET) {
				sumR += p.col.r;
				sumG += p.col.g;
				sumB += p.col.b;
				numValidNeighbors++;
			}
		}
	}

	if(numValidNeighbors == 0) {
		// TODO: this shouldn't happen, I don't think.
	}

	// by adding halfNumValidNeighbors, the result of the division will be rounded instead of floored
	uint_fast8_t halfNumValidNeighbors = numValidNeighbors / 2;
	return (Color) {
		.r = (sumR + halfNumValidNeighbors) / numValidNeighbors,
		.g = (sumG + halfNumValidNeighbors) / numValidNeighbors,
		.b = (sumB + halfNumValidNeighbors) / numValidNeighbors
	};
}

Color reserveIdealAllowedColor(Color c, ColorList* availableColors) {
	Color* cList = availableColors->list;
	size_t cListLen = availableColors->len;

	size_t bestIndex = 0;
	uint_fast32_t bestDistSquared = 0xFFFFFFFF;

	for(size_t i = 0; i < cListLen; i++) {
		Color toTest = cList[i];
		// Will this cause an error since I'm subtracting uints before converting them to ints?
		int_fast16_t dR = c.r - toTest.r;
		int_fast16_t dG = c.g - toTest.g;
		int_fast16_t dB = c.b - toTest.b;
		uint_fast32_t distSquared = dR * dR + dG * dG + dB * dB;

		if(distSquared < bestDistSquared) {
			bestDistSquared = distSquared;
			bestIndex = i;
		}
	}

	// TODO: This shouldn't happen here, but it has to for this method.
	Color ret = cList[bestIndex];
	cList[bestIndex] = cList[cListLen - 1];
	availableColors->len--;

	return ret;
}

void addPixelsToQueue(Coord lastSet, PixelMap pMap, CoordQueue* cQueue) {
	for(int dx = -1; dx <= 1; dx++) {
		for(int dy = -1; dy <= 1; dy++) {
			if(dx == 0 && dy == 0) continue;
			Coord toAdd = {
				.x = lastSet.x + dx,
				.y = lastSet.y + dy
			};
			Pixel* coordInfo = getPixelAtCoord(toAdd, pMap);

			if(coordInfo != NULL && coordInfo->status == PIXEL_STATUS_UNTOUCHED) {
				cQueue->coords[cQueue->len] = toAdd;
				cQueue->len++;
				coordInfo->status = PIXEL_STATUS_QUEUED;
			}
		}
	}
}

// Color convertToDisplayColor(Color from, ColorTransformation transform) {
// 	return {
// 		.r = (from.r * transform.r_scale) + transform.r_translation,
// 		.g = (from.g * transform.g_scale) + transform.g_translation,
// 		.b = (from.b * transform.b_scale) + transform.b_translation
// 	};
// }

void setPixelColor(Coord loc, Color col, PixelMap pMap, CoordQueue* cQueue, Color colorScale) {
	if(loc.x < 0 || loc.x >= pMap.width || loc.y < 0 || loc.y >= pMap.height) {
		// TODO: something bad?
		return;
	}
	Pixel* toSet = getPixelAtCoord(loc, pMap);
	if(toSet->status == PIXEL_STATUS_SET) {
		// TODO: ?
		return;
	}
	toSet->status = PIXEL_STATUS_SET;
	toSet->col = col;
	
	addPixelsToQueue(loc, pMap, cQueue);

	Color realColor = {
		.r = col.r * colorScale.r,
		.g = col.g * colorScale.g,
		.b = col.b * colorScale.b
	};

	displayAddPixel(realColor, loc);
}

void generateNextPixel(PixelMap pMap, CoordQueue* cQueue, ColorList* cList, Color colorScale) {
	Coord nextPixelCoord = chooseNextPixelCoord(cQueue);
	Color preferredColor = getPreferredColor(nextPixelCoord, pMap);
	Color closestAllowedColor = reserveIdealAllowedColor(preferredColor, cList);
	setPixelColor(nextPixelCoord, closestAllowedColor, pMap, cQueue, colorScale);
}

void Rainbow_Finish(PixelMap pMap, CoordQueue cQueue, ColorList cList) {
	printf("Finishing rainbow generation\n");

	free(pMap.map[0]);
	free(pMap.map);
	free(cQueue.coords);
	free(cList.list);
}

int initialize(crd_cmp width, crd_cmp height, rb_max_size_type maxR, rb_max_size_type maxG, rb_max_size_type maxB) {
	if(width * height != maxR * maxG * maxB) {
		fprintf(stderr,
			"Error initializing rainbow! width * height must be equal to maxR * maxG * maxB\n"
			"width * height == %d * %d == %d\n"
			"maxR * maxG * maxB == %d * %d * %d == %d\n",
			width, height, width * height, maxR, maxG, maxB, maxR * maxG * maxB
		);
		return 1;
	}

	if(256 % maxR != 0 || 256 % maxG != 0 || 256 % maxB != 0) {
		fprintf(stderr,
			"Error initializing rainbow! maxR, maxG, and maxB must all be divisors of 256\n"
			"maxR = %d, maxG = %d, maxB = %d\n",
			maxR, maxG, maxB
		);
		return 2;
	}

	rb_max_size_type numPixels = width * height;
	Color colorScale = {
		.r = 256 / maxR,
		.g = 256 / maxG,
		.b = 256 / maxB
	};

	uintmax_t seed = (uintmax_t) time(NULL);
	srand(seed);
	printf("Seed: %ju\n", seed);

	

	Pixel** pixelMapXIndexes = calloc(width, sizeof(Pixel*));
	Pixel* pixelMapData = calloc(numPixels, sizeof(Pixel));

	for(int x = 0; x < width; x++) {
		pixelMapXIndexes[x] = pixelMapData + (height * x);
	}
	PixelMap pixelMap = {
		.map = pixelMapXIndexes,
		.width = width,
		.height = height
	};

	Coord* queueData = calloc(numPixels, sizeof(Coord));
	CoordQueue coordQueue = {
		.coords = queueData,
		.len = 0
	};

	Color* cListData = calloc(numPixels, sizeof(Color));

	rb_max_size_type cListIndex = 0;
	for(rb_max_size_type r = 0; r < maxR; r++) {
		for(rb_max_size_type g = 0; g < maxG; g++) {
			for(rb_max_size_type b = 0; b < maxB; b++) {
				cListData[cListIndex] = (Color) {
					.r = r,
					.g = g,
					.b = b
				};
				cListIndex++;
			}
		}
	}

	ColorList colorList = {
		.list = cListData,
		.len = numPixels
	};

	setPixelColor(
		(Coord) { .x = width / 2, .y = height / 2 },
		(Color) { .r = maxR / 2, .g = maxG / 2, .b = maxB / 2 },
		pixelMap,
		&coordQueue,
		colorScale
	);

	rb_max_size_type px_num = 0;
	while(coordQueue.len > 0) {
		if(px_num == 4) {
			int trash;
			//scanf("%d", &trash);
		}
		generateNextPixel(pixelMap, &coordQueue, &colorList, colorScale);
		px_num++;
	}

	Rainbow_Finish(pixelMap, coordQueue, colorList);

	return 0;
}

int main(int argc, char** argv) {
	initializeDisplay();

	initialize(64, 64, 16, 16, 16);

	while(handleEvents());

	return 0;
}