#include "headers/RB_AssignmentQueue.h"
#include <stdlib.h>
#include <stdio.h>

struct RB_AssignmentQueue_s {
	RB_Pixel** data;
	RB_Size size;
};

// Allocates an assignmentQueue capable of storing the specified number of pixels.
RB_AssignmentQueue* RB_createAssignmentQueue(RB_Size size) {
	RB_AssignmentQueue* ret = malloc(
		sizeof(RB_AssignmentQueue)
		+ (sizeof(RB_Pixel*) * size)
	);

	if(ret == NULL) {
		return NULL;
	}

	ret->data = (RB_Pixel**) (ret + 1);
	ret->size = 0;

	return ret;
}

// Frees a previously allocated assignmentQueue
void RB_freeAssignmentQueue(RB_AssignmentQueue* queue) {
	printf("Freeing RB_AssignmentQueue!\n");
	free(queue);
}

// Returns true if the queue is empty. Otherwise, returns false.
bool RB_isQueueEmpty(RB_AssignmentQueue* queue) {
	return queue->size == 0;
}

RB_Size RB_getQueueSize(RB_AssignmentQueue* queue) {
	return queue->size;
}

// Chooses (using an implementation-specific method) a pixel from the queue and returns it.
RB_Pixel* RB_getPixelFromAssignmentQueue(RB_AssignmentQueue* queue) {
	if(RB_isQueueEmpty(queue)) {
		fprintf(stderr, "AssignmentQueue has no more pixels!\n");
		return NULL;
	}

	RB_Size retIndex = ((RB_Size) rand()) % queue->size;
	return queue->data[retIndex];
}

// If the pixel is in the Queue, removes it.
void RB_removePixelFromAssignmentQueue(RB_AssignmentQueue* queue, RB_Pixel* pixel) {

	RB_Size pixelIndex = pixel->queueData;

	if(
		(pixel->status == RB_PIXEL_QUEUED) // Make sure pixel is queued
		&& (pixelIndex >= 0) // Make sure pixel claims to be in a valid index
		&& (pixelIndex < queue->size)
		// Make sure the pixel truly is pointing to its own position. If this is not true for a queued pixel, a bug has occurred.
		&& (RB_coordsAreEqual(queue->data[pixelIndex]->loc, pixel->loc))
	) {
		RB_Pixel* lastPixel = queue->data[queue->size - 1];
		queue->data[pixelIndex] = lastPixel;
		lastPixel->queueData = pixelIndex;
		pixel->status = RB_PIXEL_BLANK;
		queue->size--;
	} else {
		fprintf(stderr, "Pixel is not Queued or has incorrect queue data.\n");
	}
}

/*
- If the pixel is not already queued or assigned, adds the pixel to the queue (with the specified priority, if
the RB_AssignmentQueue implementation supports prioritization).
- If the pixel is already queued and the RB_AssignmentQueue implementation supports prioritization, this function may update
the pixel's priority, though that is not guaranteed.

Higher positive values for priorityIndex correspond to lower prioritization. A priorityIndex of 0 corresponds to the maximum
possible prioritization.
Negative values for priorityIndex correspond to the lowest possible prioritization.
*/
void RB_addPixelToAssignmentQueue(RB_AssignmentQueue* queue, RB_Pixel* toAdd, RB_Size priorityIndex) {
	switch(toAdd->status) {
		case RB_PIXEL_BLANK:
			queue->data[queue->size] = toAdd;
			toAdd->status = RB_PIXEL_QUEUED;
			toAdd->queueData = queue->size;
			queue->size++;
			return;
		case RB_PIXEL_SET:
			fprintf(stderr, "Attemping to queue an already-set pixel!\n");
			return;
		default:
			return;
	}
}

void RB_printAssignmentQueue(RB_AssignmentQueue* aaa) {

}