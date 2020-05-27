#ifndef EKW_RAINBOW_IMAGE_GEN_H
#define EKW_RAINBOW_IMAGE_GEN_H

#include <stdint.h>

typedef int_fast32_t rb_max_size_type;

typedef int_fast16_t crd_cmp; // coordinate component

typedef struct {
	crd_cmp x;
	crd_cmp y;
} Coord;

typedef uint_fast16_t color_size_type; // Type big enough to represent the bounds of a color channel.
typedef uint_fast8_t clr_ch; // color channel

typedef struct {
	clr_ch r;
	clr_ch g;
	clr_ch b;
} Color;


//int initialize(crd_cmp, crd_cmp, rb_max_size_type, rb_max_size_type, rb_max_size_type);

#endif