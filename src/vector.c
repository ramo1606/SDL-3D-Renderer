#include <math.h>
#include "vector.h"

///////////////////////////////////////////////////////////////////////////////
// Implementations of Vector 2D functions
///////////////////////////////////////////////////////////////////////////////

/* Calculate the length of a 2D vector */
float vec2_length(vec2_t v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

/* Add two 2D vectors */
vec2_t vec2_add(vec2_t a, vec2_t b)
{
    vec2_t result =
    {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
    return result;
}

/* Subtract two 2D vectors */
vec2_t vec2_sub(vec2_t a, vec2_t b)
{
    vec2_t result =
    {
        .x = a.x - b.x,
        .y = a.y - b.y
    };
    return result;
}

/* Multiply a 2D vector by a scalar */
vec2_t vec2_mul(vec2_t v, float factor)
{
    vec2_t result =
    {
        .x = v.x * factor,
        .y = v.y * factor
    };
    return result;
}

/* Divide a 2D vector by a scalar */
vec2_t vec2_div(vec2_t v, float factor)
{
    vec2_t result =
    {
        .x = v.x / factor,
        .y = v.y / factor
    };
    return result;
}

/* Calculate the dot product of two 2D vectors */
float vec2_dot(vec2_t a, vec2_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}

/* Normalize a 2D vector */
void vec2_normalize(vec2_t* v)
{
    float length = sqrt(v->x * v->x + v->y * v->y);
    v->x /= length;
    v->y /= length;
}

///////////////////////////////////////////////////////////////////////////////
// Implementations of Vector 3D functions
///////////////////////////////////////////////////////////////////////////////

/* Calculate the length of a 3D vector */
float vec3_length(vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

/* Add two 3D vectors */
vec3_t vec3_add(vec3_t a, vec3_t b)
{
    vec3_t result =
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
    return result;
}

/* Subtract two 3D vectors */
vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    vec3_t result =
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
    return result;
}

/* Multiply a 3D vector by a scalar */
vec3_t vec3_mul(vec3_t v, float factor)
{
    vec3_t result =
    {
        .x = v.x * factor,
        .y = v.y * factor,
        .z = v.z * factor
    };
    return result;
}

/* Divide a 3D vector by a scalar */
vec3_t vec3_div(vec3_t v, float factor)
{
    vec3_t result =
    {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z / factor
    };
    return result;
}

/* Calculate the cross product of two 3D vectors */
vec3_t vec3_cross(vec3_t a, vec3_t b)
{
    vec3_t result =
    {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };
    return result;
}

/* Calculate the dot product of two 3D vectors */
float vec3_dot(vec3_t a, vec3_t b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/* Normalize a 3D vector */
void vec3_normalize(vec3_t* v)
{
    float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

/* Rotate a 3D vector around the X-axis */
vec3_t vec3_rotate_x(vec3_t v, float angle)
{
    vec3_t rotated_vector =
    {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

/* Rotate a 3D vector around the Y-axis */
vec3_t vec3_rotate_y(vec3_t v, float angle)
{
    vec3_t rotated_vector =
    {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

/* Rotate a 3D vector around the Z-axis */
vec3_t vec3_rotate_z(vec3_t v, float angle)
{
    vec3_t rotated_vector =
    {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
    return rotated_vector;
}

/* Convert vector 3 to vector 4 */
vec4_t vec4_from_vec3(vec3_t v)
{
	vec4_t result =
	{
		.x = v.x,
		.y = v.y,
		.z = v.z,
		.w = 1.f
	};
    return result;
}

/* Convert vector 4 to vector 3 */
vec3_t vec3_from_vec4(vec4_t v)
{
	vec3_t result =
	{
		.x = v.x,
		.y = v.y,
		.z = v.z
	};
    return result;
}
