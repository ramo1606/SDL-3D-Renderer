#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"

/* Structure to represent a face with three vertices */
typedef struct
{
    int a;
    int b;
    int c;
    uint32_t color;
} face_t;

/* Structure to represent a triangle with three 2D points */
typedef struct
{
    vec2_t points[3];
	uint32_t color;
	float avg_depth;
} triangle_t;

#endif /* TRIANGLE_H */