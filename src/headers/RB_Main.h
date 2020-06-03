#ifndef EKW_RAINBOW_RB_DATA_H
#define EKW_RAINBOW_RB_DATA_H

#include "RB_BasicTypes.h"
#include <stdbool.h>

// forward declaring structs here because the public-facing part of the library doesn't need to know their functions.
// In fact, the forward facing part might not even need to know the contents of RB_Data. That's something to consider.
typedef struct RB_AssignmentQueue_s RB_AssignmentQueue;
typedef struct RB_ColorPool_s RB_ColorPool;
typedef struct RB_PixelMap_s RB_PixelMap;
typedef struct RB_Display_s RB_Display;

typedef struct RB_Data_s RB_Data;

struct RB_Data_s {
	RB_AssignmentQueue* assignmentQueue; // the queue of coordinates that should be assigned a color.
	RB_ColorPool* colorPool;
	RB_PixelMap* pixelMap;
	RB_Display* display;
};

typedef struct {
	RB_Size width;
	RB_Size height;

	RB_ColorChannelSize rRes;
	RB_ColorChannelSize gRes;
	RB_ColorChannelSize bRes;

	int windowWidth;
	int windowHeight;
} RB_Config;

RB_Data* RB_init(RB_Config);

void RB_free(RB_Data*);


// Sets the color for another pixel. Returns true if there are pixels left to generate, otherwise returns false.
bool RB_generateNextPixel(RB_Data*);

#endif