#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>
#include "vector.h"

typedef struct
{
	vec3_t direction;
	/*vec3_t color;
	float intensity;*/
} light_t;

extern light_t light;

/* Function to apply light intensity to a color */
uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);

#endif // !LIGHT_H

