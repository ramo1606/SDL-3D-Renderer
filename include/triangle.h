#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>
#include "vector.h"
#include "texture.h"

/* Structure to represent a face with three vertices */
typedef struct
{
    int a;
    int b;
    int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
    uint32_t color;
} face_t;

/* Structure to represent a triangle with three 2D points */
typedef struct
{
    vec4_t points[3];
	tex2_t texcoords[3];
	uint32_t color;
	float avg_depth;
} triangle_t;

#endif /* TRIANGLE_H */