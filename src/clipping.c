#include "clipping.h"
#include <math.h>
#include <string.h>

#define NUM_FRUSTUM_PLANES 6
plane_t frustum_planes[NUM_FRUSTUM_PLANES];

// Frustum planes are defined by a point and a normal vector
// Near plane		:		Point = (0, 0, z_near), Normal = (0, 0, 1)
// Far plane		:		Point = (0, 0, z_far),	Normal = (0, 0, -1)
// Left plane		:		Point = (0, 0, 0),		Normal = (cos(fov/2), 0, sin(fov/2))
// Right plane		:		Point = (0, 0, 0),		Normal = (-cos(fov/2), 0, sin(fov/2))
// Bottom plane		:		Point = (0, 0, 0),		Normal = (0, cos(fov/2), sin(fov/2))
// Top plane		:		Point = (0, 0, 0),		Normal = (0, -cos(fov/2), sin(fov/2))
void init_frustum_planes(float fov, float z_near, float z_far)
{
	float cos_half_fov = cosf(fov / 2.0f);
	float sin_half_fov = sinf(fov / 2.0f);

	vec3_t origin = { 0.0f, 0.0f, 0.0f };

	// Near plane
	frustum_planes[NEAR_FRUSTUM_PLANE].point = (vec3_t) { 0.f, 0.f, z_near };
	frustum_planes[NEAR_FRUSTUM_PLANE].normal = (vec3_t){ 0.0f, 0.0f, 1.0f };

	// Far plane
	frustum_planes[FAR_FRUSTUM_PLANE].point = (vec3_t){ 0.f, 0.f, z_far };
	frustum_planes[FAR_FRUSTUM_PLANE].normal = (vec3_t){ 0.0f, 0.0f, -1.0f };

	// Left plane
	frustum_planes[LEFT_FRUSTUM_PLANE].point = origin;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal = (vec3_t){ cos_half_fov, 0.0f, sin_half_fov };

	// Right plane
	frustum_planes[RIGHT_FRUSTUM_PLANE].point = origin;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal = (vec3_t){ -cos_half_fov, 0.0f, sin_half_fov };

	// Bottom plane
	frustum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal = (vec3_t){ 0.0f, cos_half_fov, sin_half_fov };

	// Top plane
	frustum_planes[TOP_FRUSTUM_PLANE].point = origin;
	frustum_planes[TOP_FRUSTUM_PLANE].normal = (vec3_t){ 0.0f, -cos_half_fov, sin_half_fov };
}

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2)
{
	polygon_t polygon;

	polygon.vertices[0] = v0;
	polygon.vertices[1] = v1;
	polygon.vertices[2] = v2;

	polygon.num_vertices = 3;

	return polygon;
}

void clip_polygon_against_plane(polygon_t* polygon, int plane_index)
{
	vec3_t plane_point = frustum_planes[plane_index].point;
	vec3_t plane_normal = frustum_planes[plane_index].normal;

	// The array of inside vertices will be used to store the vertices that are inside the plane
	vec3_t inside_vertices[MAX_NUM_POLY_VERTICES];
	int num_inside_vertices = 0;

	// Start current and previous vertex witht the first and last polygon vertices
	vec3_t* current_vertex = &polygon->vertices[0];
	vec3_t* previous_vertex = &polygon->vertices[polygon->num_vertices - 1];

	// Start the current and previous distance with the dot product of the first and last vertices with the plane normal
	float current_dot = 0.0f;
	float previous_dot = vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

	// Loop through all the polygon vertices
	while (current_vertex != &polygon->vertices[polygon->num_vertices]) 
	{
		current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

		// If we change from outside to inside or inside to outside
		if (current_dot * previous_dot < 0.0f)
		{
			// Calculate the intersection point
			float t = previous_dot / (previous_dot - current_dot); // Linear interpolation factor
			vec3_t intersection_point = vec3_add(*previous_vertex, vec3_mul(vec3_sub(*current_vertex, *previous_vertex), t)); // I = Q1 + t(Q2 - Q1)

			// Insert the intersection point into the inside vertices array
			inside_vertices[num_inside_vertices] = intersection_point;
			num_inside_vertices++;
		}
		
		// If the current vertex is inside the plane
		if (current_dot > 0.0f) 
		{
			// Insert the current vertex into the inside vertices array
			inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
			num_inside_vertices++;
		}

		// Move to the next vertex
		previous_vertex = current_vertex;
		previous_dot = current_dot;
		current_vertex++;
	}

	// Copy the inside vertices into the polygon vertices array
	memcpy(polygon->vertices, inside_vertices, num_inside_vertices * sizeof(vec3_t));
	polygon->num_vertices = num_inside_vertices;
}

void clip_polygon(polygon_t* polygon)
{
	clip_polygon_against_plane(polygon, LEFT_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, RIGHT_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, BOTTOM_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, TOP_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, NEAR_FRUSTUM_PLANE);
	clip_polygon_against_plane(polygon, FAR_FRUSTUM_PLANE);
}
