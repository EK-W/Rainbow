#include "headers/RB_PixelMap.h"
#include "headers/RB_AssignmentQueue.h"
#include <stdlib.h>
#include <stdio.h>

struct RB_PixelMap_s {
	RB_Pixel** pixels;
	RB_Size width;
	RB_Size height;
};

// allocates a pixel map with the specified dimensions
RB_PixelMap* RB_createPixelMap(RB_Size width, RB_Size height) {
	RB_PixelMap* ret = (RB_PixelMap*) malloc(
		sizeof(RB_PixelMap)
		+ (sizeof(RB_Pixel*) * width)
		+ (sizeof(RB_Pixel) * width * height)
	);

	if(ret == NULL) {
		return NULL;
	}

	ret->width = width;
	ret->height = height;

	ret->pixels = (RB_Pixel**) (ret + 1);
	RB_Pixel* pixelData = (RB_Pixel*) (ret->pixels + width);

	for(int x = 0; x < width; x++) {
		ret->pixels[x] = pixelData + (x * height);

		for(int y = 0; y < height; y++) {
			ret->pixels[x][y] = (RB_Pixel) {
				.loc = { .x = x, .y = y },
				.color = { .r = 0, .g = 0, .b = 0 },
				.status = RB_PIXEL_BLANK
			};
		}
	}

	return ret;
}

// deallocates the pixel map
void RB_freePixelMap(RB_PixelMap* map) {
	printf("Freeing RB_PixelMap!\n");
	free(map);
}

// returns the pixel that the coord maps to, or NULL if the coord does not map to a pixel.
RB_Pixel* RB_getPixel(RB_PixelMap* map, RB_Coord coord) {
	if(coord.x < 0 || coord.x >= map->width || coord.y < 0 || coord.y >= map->height) {
		return NULL;
	}

	return &(map->pixels[coord.x][coord.y]);
}

// Determines, based on the current state of the pixelMap, the preferred color for the specified coordinate.
RB_Color RB_determinePreferredCoordColor(RB_PixelMap* pixelMap, RB_Coord coord) {
	RB_Size minX = ((coord.x - 1) < 0)? 0 : coord.x - 1;
	RB_Size maxX = ((coord.x + 1) >= pixelMap->width)? pixelMap->width - 1 : coord.x + 1;
	RB_Size minY = ((coord.y - 1) < 0)? 0 : coord.y - 1;
	RB_Size maxY = ((coord.y + 1) >= pixelMap->height)? pixelMap->height - 1 : coord.y + 1;

	// A ColorChannelSum is garanteed to be able to hold the sum of up to 256 colorChannel values
	// Because we're adding half of numNeighbors, this will have a lower capacity.
	// That doesn't matter here, though.
	RB_ColorChannelSum rSum = 0;
	RB_ColorChannelSum gSum = 0;
	RB_ColorChannelSum bSum = 0;

	uint_fast8_t numNeighbors = 0;

	for(RB_Size x = minX; x <= maxX; x++) {
		for(RB_Size y = minY; y <= maxY; y++) {
			RB_Coord newCoord = { .x = x, .y = y };

			RB_Pixel* neighborPixel = RB_getPixel(pixelMap, newCoord);

			if(neighborPixel == NULL) {
				fprintf(stderr,
					"Somehow, determinePreferredCoordColor has encountered a NULL pixel"
					"even though that shouldn't be possible?\nThe pixel is at %d %d.\n",
					x, y
				);
				continue;
			}

			if(neighborPixel->status != RB_PIXEL_SET) continue;

			numNeighbors++;
			rSum += neighborPixel->color.r;
			gSum += neighborPixel->color.g;
			bSum += neighborPixel->color.b;
		}
	}

	uint_fast8_t halfNumNeighbors = numNeighbors / 2;
	// By adding half of numNeighbors, hopefully the sums will
	return (RB_Color) {
		.r = (rSum + halfNumNeighbors) / numNeighbors,
		.g = (gSum + halfNumNeighbors) / numNeighbors,
		.b = (bSum + halfNumNeighbors) / numNeighbors
	};
}

// Add cords to the queue in an implementation-defined pattern relative to the given coord
void RB_addResultantCoordsToQueue(RB_PixelMap* map, RB_AssignmentQueue* queue, RB_Coord center) {
	for(RB_Size dx = -1; dx <= 1; dx++) {
		for(RB_Size dy = -1; dy <= 1; dy++) {
			if(dx == 0 && dy == 0) continue;


			RB_Pixel* toAdd = RB_getPixel(map, (RB_Coord) { .x = center.x + dx, .y = center.y + dy });
			if(toAdd == NULL) continue;
			if(toAdd->status != RB_PIXEL_BLANK) continue;

			RB_addCoordToAssignmentQueue(queue, toAdd->loc, -1);
		}
	}
}