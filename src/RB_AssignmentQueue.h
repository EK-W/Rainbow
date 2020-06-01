#ifndef EKW_RAINBOW_RB_ASSIGNMENT_QUEUE_H
#define EKW_RAINBOW_RB_ASSIGNMENT_QUEUE_H

#include "RB_Main.h"
#include "RB_Pixel.h"
#include "RB_BasicTypes.h"

// Allocates an assignmentQueue capable of storing the specified number of pixels.
RB_AssignmentQueue* RB_createAssignmentQueue(RB_Size);

// Frees a previously allocated assignmentQueue
void RB_freeAssignmentQueue(RB_AssignmentQueue*);

// Returns true if the queue is empty. Otherwise, returns false.
bool RB_isQueueEmpty(RB_AssignmentQueue*);

RB_Size RB_getQueueSize(RB_AssignmentQueue*);

// Chooses (using an implementation-specific method) a pixel from the queue and returns it.
RB_Pixel* RB_getPixelFromAssignmentQueue(RB_AssignmentQueue*);

// If the pixel is in the Queue, removes it.
void RB_removePixelFromAssignmentQueue(RB_AssignmentQueue*, RB_Pixel*);

/*
- If the pixel is not already queued or assigned, adds the pixel to the queue (with the specified priority, if
the RB_AssignmentQueue implementation supports prioritization).
- If the pixel is already queued and the RB_AssignmentQueue implementation supports prioritization,this function may update
the pixel's priority, though that is not guaranteed.

Higher positive values for priorityIndex correspond to lower prioritization. A priorityIndex of 0 corresponds to the maximum
possible prioritization.
Negative values for priorityIndex correspond to the lowest possible prioritization.
*/
void RB_addPixelToAssignmentQueue(RB_AssignmentQueue*, RB_Pixel*, RB_Size priorityIndex);

void RB_printAssignmentQueue(RB_AssignmentQueue*);


#endif