#ifndef EKW_RAINBOW_DISPLAY_H
#define EKW_RAINBOW_DISPLAY_H

#include "rainbowImageGen.h"


int initializeDisplay();
int handleEvents();
void displayAddPixel(Color c, Coord loc);



#endif