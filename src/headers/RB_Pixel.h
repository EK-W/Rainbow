#ifndef EKW_RAINBOW_RB_PIXEL_H
#define EKW_RAINBOW_RB_PIXEL_H

#include "RB_BasicTypes.h"


typedef enum {
	RB_PIXEL_BLANK,
	RB_PIXEL_QUEUED,
	RB_PIXEL_SET
} RB_PixelStatus;

typedef struct {
	RB_Coord loc;

	RB_Color color;

	// A variable used by the assignmentQueue to keep track of the Pixel's location in the queue.
	RB_Size queueData;
	
	RB_PixelStatus status;
} RB_Pixel;

#endif