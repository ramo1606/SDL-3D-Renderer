#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

/* Structure to represent a face with three vertices */
typedef struct
{
    int a;
    int b;
    int c;
} face_t;

/* Structure to represent a triangle with three 2D points */
typedef struct
{
    vec2_t points[3];
} triangle_t;

#endif /* TRIANGLE_H */