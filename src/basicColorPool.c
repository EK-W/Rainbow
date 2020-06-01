#include "RB_ColorPool.h"
#include <stdlib.h>
#include <stdio.h>

struct RB_ColorPool_s {
	RB_Color* colors;
	RB_Size colorsSize;

	RB_Size*** colorIndexes;
	RB_ColorChannelSize rSize;
	RB_ColorChannelSize gSize;
	RB_ColorChannelSize bSize;
};

// Allocates a colorPool with the specified range of colors.
RB_ColorPool* RB_createColorPool(RB_ColorChannelSize rSize, RB_ColorChannelSize gSize, RB_ColorChannelSize bSize) {
	RB_ColorPool* ret = (RB_ColorPool*) malloc(
		sizeof(RB_ColorPool) // The struct itself
		+ (sizeof(RB_Color) * rSize * gSize * bSize) // the color list.
		+ (sizeof(RB_Size**) * rSize) // The color indexes
		+ (sizeof(RB_Size*) * rSize * gSize)
		+ (sizeof(RB_Size) * rSize * gSize * bSize)
	);

	if(ret == NULL) {
		return NULL;
	}

	ret->colors = (RB_Color*) (ret + 1);
	ret->colorsSize = rSize * gSize * bSize;

	ret->colorIndexes = (RB_Size***) (ret->colors + (rSize * gSize * bSize));
	RB_Size** gIndexesStart = (RB_Size**) (ret->colorIndexes + rSize);
	RB_Size* bIndexesStart = (RB_Size*) (gIndexesStart + (rSize * gSize));

	RB_Size cIndex = 0;
	for(RB_ColorChannelSize r = 0; r < rSize; r++) {
		ret->colorIndexes[r] = gIndexesStart + (r * gSize);

		for(RB_ColorChannelSize g = 0; g < gSize; g++) {
			ret->colorIndexes[r][g] = bIndexesStart + (((r * gSize) + g) * bSize);

			for(RB_ColorChannelSize b = 0; b < bSize; b++) {
				// This shouldn't overflow because rSize, gSize, and bSize should never be more than 256 
				ret->colors[cIndex] = (RB_Color) { .r = r, .g = g, .b = b };
				ret->colorIndexes[r][g][b] = cIndex;
				cIndex++;
			}
		}
	}

	return ret;
}

// Frees a previously allocated color pool
void RB_freeColorPool(RB_ColorPool* pool) {
	printf("Freeing RB_ColorPool!\n");
	free(pool);
}

RB_Color RB_findIdealAvailableColor(RB_ColorPool* pool, RB_Color preferredColor) {
	RB_ColorSquareDistance bestDistanceSq = ~0; // The inverse of 0 is the largest possible value for the type.
	RB_Size bestIndex = -1;
	for(RB_Size i = 0; i < pool->colorsSize; i++) {
		RB_Color iColor = pool->colors[i];
		RB_ColorChannelDifference deltaR = preferredColor.r - iColor.r;
		RB_ColorChannelDifference deltaG = preferredColor.g - iColor.g;
		RB_ColorChannelDifference deltaB = preferredColor.b - iColor.b;

		RB_ColorSquareDistance distSq = (deltaR * deltaR) + (deltaG * deltaG) + (deltaB * deltaB);

		if(distSq < bestDistanceSq) {
			bestDistanceSq = distSq;
			bestIndex = i;
		}
	}

	return pool->colors[bestIndex];
}

bool RB_colorIsAvailableInPool(RB_ColorPool* pool, RB_Color color) {
	return (pool->colorIndexes[color.r][color.g][color.b] != -1);
}

void RB_removeColorFromPool(RB_ColorPool* pool, RB_Color toRemove) {
	RB_Size colorIndex = pool->colorIndexes[toRemove.r][toRemove.g][toRemove.b];

	if(colorIndex == -1) {
		fprintf(stderr, "Error: attempting to remove color that has already been removed.\n");
		return;
	}

	pool->colorIndexes[toRemove.r][toRemove.g][toRemove.b] = -1;

	RB_Color lastColor = pool->colors[pool->colorsSize - 1];

	pool->colorIndexes[lastColor.r][lastColor.g][lastColor.b] = colorIndex;
	pool->colors[colorIndex] = lastColor;

	pool->colorsSize--;
}