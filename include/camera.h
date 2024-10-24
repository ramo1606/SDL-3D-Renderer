#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

/* Structure to represent a camera with position and direction */
typedef struct
{
	vec3_t position;
	vec3_t direction;
	vec3_t forward_velocity;
	float yaw;
} camera_t;

extern camera_t camera;

#endif /* CAMERA_H */
