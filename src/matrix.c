#include <math.h>
#include "matrix.h"

mat4_t mat4_identity(void)
{
	// | 1 0 0 0 |
	// | 0 1 0 0 |
	// | 0 0 1 0 |
	// | 0 0 0 1 |

	mat4_t m = {{
			{ 1, 0, 0, 0 }, // row 1
			{ 0, 1, 0, 0 }, // row 2
			{ 0, 0, 1, 0 }, // row 3
			{ 0, 0, 0, 1 }  // row 4
	}};
	return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
	// | sx  0  0  0 |
	// |  0 sy  0  0 |
	// |  0  0 sz  0 |
	// |  0  0  0  1 |
	mat4_t m = mat4_identity();
	m.m[0][0] = sx;
	m.m[1][1] = sy;
	m.m[2][2] = sz;
	return m;
}

mat4_t mat4_make_translation(float tx, float ty, float tz)
{
	// | 1  0  0  tx |
	// | 0  1  0  ty |
	// | 0  0  1  tz |
	// | 0  0  0  1  |
	mat4_t m = mat4_identity();
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	m.m[2][3] = tz;
	return m;
}

mat4_t mat4_make_rotation_x(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	// | 1  0  0  0 |
	// | 0  c -s  0 |
	// | 0  s  c  0 |
	// | 0  0  0  1 |
	mat4_t m = mat4_identity();
	m.m[1][1] = c;
	m.m[1][2] = -s;
	m.m[2][1] = s;
	m.m[2][2] = c;
	return m;
}

mat4_t mat4_make_rotation_y(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	// |  c  0  s  0 |
	// |  0  1  0  0 |
	// | -s  0  c  0 |
	// |  0  0  0  1 |
	mat4_t m = mat4_identity();
	m.m[0][0] = c;
	m.m[0][2] = s;
	m.m[2][0] = -s;
	m.m[2][2] = c;
	return m;
}

mat4_t mat4_make_rotation_z(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);
	// |  c -s  0  0 |
	// |  s  c  0  0 |
	// |  0  0  1  0 |
	// |  0  0  0  1 |
	mat4_t m = mat4_identity();
	m.m[0][0] = c;
	m.m[0][1] = -s;
	m.m[1][0] = s;
	m.m[1][1] = c;
	return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float near, float far)
{
	float f = 1.0f / tanf(fov * 0.5f);
	// | aspect*f        0                0                      0 |
	// |        0        f                0                      0 |
	// |        0        0 (far)/(far-near) (-far*near)/(far-near) |
	// |        0        0                1                      0 |
	mat4_t m = { {
			{ aspect * f, 0, 0, 0 },
			{ 0, f, 0, 0 },
			{ 0, 0, far / (far - near), (-far * near) / (far - near)},
			{ 0, 0, 1.f, 0 }
	} };
	return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
	vec4_t result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
	return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b)
{
	mat4_t result;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			result.m[row][col] = a.m[row][0] * b.m[0][col] +
				a.m[row][1] * b.m[1][col] +
				a.m[row][2] * b.m[2][col] +
				a.m[row][3] * b.m[3][col];
		}
	}
	return result;
}

vec4_t mat4_mul_vec4_project(mat4_t m, vec4_t v)
{
	vec4_t result = mat4_mul_vec4(m, v);
	if (result.w != 0)
	{
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}

mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up)
{
	vec3_t z = vec3_sub(target, eye);
	vec3_normalize(&z);
	vec3_t x = vec3_cross(up, z);
	vec3_normalize(&x);
	vec3_t y = vec3_cross(z, x);

	// | x.x  x.y  x.z  -dot(x, eye) |
	// | y.x  y.y  y.z  -dot(x, eye) |
	// | z.x  z.y  z.z  -dot(x, eye) |
	// |  0    0    0         1      |
	mat4_t view_matrix = { {
			{ x.x, x.y, x.z, -vec3_dot(x, eye) },
			{ y.x, y.y, y.z, -vec3_dot(y, eye) },
			{ z.x, z.y, z.z, -vec3_dot(z, eye) },
			{ 0, 0, 0, 1 }
	} };
	return view_matrix;
}
