#include "display.h"
#include "swap.h"
#include "vector.h"

/* Global variables */
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
float* depth_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

/* Function to initialize the window */
bool initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    /* Set width and height of the SDL window with the max screen resolution */
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    /* Create a SDL Window */
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    /* Create a SDL renderer */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

/* Function to draw a grid */
void draw_grid(void)
{
    for (int y = 0; y < window_height; y += 10)
    {
        for (int x = 0; x < window_width; x += 10)
        {
            color_buffer[(window_width * y) + x] = 0xFF444444;
        }
    }
}

/* Function to draw a single pixel */
void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < window_width && y >= 0 && y < window_height)
    {
        color_buffer[(window_width * y) + x] = color;
    }
}

/* Function to draw a line */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;
    for (int i = 0; i <= longest_side_length; i++)
    {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

/* Function to draw a triangle */
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

//void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
//{
//    // Calculate the inverse slopes
//	float inv_slope_1 = (float)(x1 - x0) / (float)(y1 - y0);
//	float inv_slope_2 = (float)(x2 - x0) / (float)(y2 - y0);
//
//    // Start x_start anmd x_end from the top vertex (x0, y0)
//	float x_start = x0;
//	float x_end = x0;
//
//    // Loop through the scanlines from top to bottom
//	for (int scanline_y = y0; scanline_y <= y1; scanline_y++)
//	{
//		draw_line(round(x_start), scanline_y, round(x_end), scanline_y, color);
//        x_start += inv_slope_1;
//        x_end += inv_slope_2;
//	}
//}
//
//void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
//{
//	// Calculate the inverse slopes
//	float inv_slope_1 = (float)(x2 - x0) / (float)(y2 - y0);
//	float inv_slope_2 = (float)(x2 - x1) / (float)(y2 - y1);
//
//    // Start x_start anmd x_end from the bottom vertex (x2, y2)
//	float x_start = x2;
//	float x_end = x2;
//
//    // Loop through the scanlines from bottom to top
//	for (int scanline_y = y2; scanline_y > y0; scanline_y--)
//	{
//		draw_line(round(x_start), scanline_y, round(x_end), scanline_y, color);
//        x_start -= inv_slope_1;
//        x_end -= inv_slope_2;
//	}
//}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	// Find the vectors between the vertices ABC and point P
	vec2_t ac = vec2_sub(c, a);
	vec2_t ab = vec2_sub(b, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t pc = vec2_sub(c, p);
	vec2_t pb = vec2_sub(b, p);

	// Compute the area of the parallelogram ABC using 2D cross product
	float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // |ac x ab|

	// Alpha is teh area of the small parallelogram PBC divided by the area of the ABC parallelogram
	float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

	// Beta is the area of the small parallelogram APC divided by the area of the ABC parallelogram
	float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

	// Gamma is the remaining area
	float gamma = 1.0f - alpha - beta;

	return (vec3_t) { alpha, beta, gamma };
}

void draw_triangle_pixel(int x, int y, uint32_t color, vec4_t point_a, vec4_t point_b, vec4_t point_c)
{
	vec2_t p = { x, y };
	vec2_t a = vec2_from_vec4(point_a);
	vec2_t b = vec2_from_vec4(point_b);
	vec2_t c = vec2_from_vec4(point_c);

	vec3_t weights = barycentric_weights(a, b, c, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolated_reciprocal_w = (1.0f / point_a.w) * alpha + (1.0f / point_b.w) * beta + (1.0f / point_c.w) * gamma;

	interpolated_reciprocal_w = 1.0f - interpolated_reciprocal_w;

	if (interpolated_reciprocal_w < depth_buffer[(window_width * y) + x])
	{
		draw_pixel(x, y, color);
		depth_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
	}
}

void draw_filled_triangle(
	int x0, int y0, float z0, float w0,
	int x1, int y1, float z1, float w1,
	int x2, int y2, float z2, float w2,
	uint32_t color)
{
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	/* Sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) */
	if (y0 > y1)
	{
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}

	if (y1 > y2)
	{
        int_swap(&y2, &y1);
        int_swap(&x2, &x1);
		float_swap(&z2, &z1);
		float_swap(&w2, &w1);
	}

	if (y0 > y1)
	{
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}

	/* Create vector points after we sort the vertices */
	vec4_t point_a = { x0, y0, z0, w0 };
	vec4_t point_b = { x1, y1, z1, w1 };
	vec4_t point_c = { x2, y2, z2, w2 };

	/* Render de upper part of the triangle (Flat-Bottom) */
	float inv_slope_1 = 0.f;
	float inv_slope_2 = 0.f;

	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / fabs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / fabs(y2 - y0);

	if (y1 - y0 != 0)
	{
		for (int y = y0; y <= y1; y++)
		{
			int start_x = x1 + (y - y1) * inv_slope_1;
			int end_x = x0 + (y - y0) * inv_slope_2;

			if (start_x > end_x)
			{
				int_swap(&start_x, &end_x);     // swap if start_x is to the right of end_x
			}

			for (int x = start_x; x < end_x; x++)
			{
				draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
			}

		}
	}

	/* Render de bottom part of the triangle (Flat-Top) */
	inv_slope_1 = 0.f;
	inv_slope_2 = 0.f;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / fabs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / fabs(y2 - y0);

	if (y2 - y1 != 0)
	{
		for (int y = y1; y <= y2; y++)
		{
			int start_x = x1 + (y - y1) * inv_slope_1;
			int end_x = x0 + (y - y0) * inv_slope_2;

			if (start_x > end_x)
			{
				int_swap(&start_x, &end_x);     // swap if start_x is to the right of end_x
			}

			for (int x = start_x; x < end_x; x++)
			{
				draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
			}
		}
	}
}

/* Function to draw the textured pixel at position x and y using interpolation */
void draw_texel(
    int x, int y, uint32_t* texture, 
    vec4_t point_a, vec4_t point_b, vec4_t point_c, 
    tex2_t a_uv, tex2_t b_uv, tex2_t c_uv)
{
    vec3_t weights = barycentric_weights(vec2_from_vec4(point_a), vec2_from_vec4(point_b), vec2_from_vec4(point_c), (vec2_t) { x, y });

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	// Variables to store the interpolated values of u, v and w for the current pixel
    float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;

	// Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
	interpolated_u = ((a_uv.u / point_a.w) * alpha) + ((b_uv.u / point_b.w) * beta) + ((c_uv.u / point_c.w) * gamma);
	interpolated_v = ((a_uv.v / point_a.w) * alpha) + ((b_uv.v / point_b.w) * beta) + ((c_uv.v / point_c.w) * gamma);

	// Also interpolate the reciprocal of w (1/w) for perspective correct texture mapping
	interpolated_reciprocal_w = (1.0f / point_a.w) * alpha + (1.0f / point_b.w) * beta + (1.0f / point_c.w) * gamma;

	// Divide the interpolated u and v by the interpolated reciprocal w
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	// Maps the u and v coordinates to the texture space (width and height)
	int tex_x = abs((int)(interpolated_u * tex_width)) % tex_width;
	int tex_y = abs((int)(interpolated_v * tex_height)) % tex_height;

	// Clamp the texture coordinates to the texture width and height
	// if (tex_x >= tex_width) tex_x = tex_width - 1;
	// if (tex_y >= tex_height) tex_y = tex_height - 1;

    // Get the color from the texture
	uint32_t texel_color = texture[(tex_width * tex_y) + tex_x];

	// Adjust the 1/w so the pixels that are closer to the camera have a smaller value
	interpolated_reciprocal_w = 1.0f - interpolated_reciprocal_w;

	// Check if the current pixel is closer to the camera than the previous one
	if (interpolated_reciprocal_w < depth_buffer[(window_width * y) + x])
	{
		// Draw the pixel at position x and y with the texel color
		draw_pixel(x, y, texel_color);

		// Update the depth buffer with the interpolated reciprocal w
		depth_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
	}
}

/* Function to draw a textured triangle */
void draw_textured_triangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    uint32_t* texture)
{
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	/* Sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) */
	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	if (y1 > y2)
	{
		int_swap(&y2, &y1);
		int_swap(&x2, &x1);
		float_swap(&z2, &z1);
		float_swap(&w2, &w1);
		float_swap(&u2, &u1);
		float_swap(&v2, &v1);
	}

	if (y0 > y1)
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	/* Flip the V component to account for inverted UV-Coordinates (V grows downwards) */
	v0 = 1.0f - v0;
	v1 = 1.0f - v1;
	v2 = 1.0f - v2; 

    /* Create vector points after we sort the vertices */
	vec4_t point_a = { x0, y0, z0, w0 };
	vec4_t point_b = { x1, y1, z1, w1 };
	vec4_t point_c = { x2, y2, z2, w2 };

	/* Render de upper part of the triangle (Flat-Bottom) */
	float inv_slope_1 = 0.f;
	float inv_slope_2 = 0.f;

    if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / fabs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / fabs(y2 - y0);

    if (y1 - y0 != 0) 
    {
        for (int y = y0; y <= y1; y++) 
        {
			int start_x = x1 + (y - y1) * inv_slope_1;
			int end_x = x0 + (y - y0) * inv_slope_2;

			if (start_x > end_x)
			{
				int_swap(&start_x, &end_x);     // swap if start_x is to the right of end_x
			}

			for (int x = start_x; x < end_x; x++)
			{
				draw_texel(x, y, texture, point_a, point_b, point_c, (tex2_t) { u0, v0 }, (tex2_t) { u1, v1 }, (tex2_t) { u2, v2 });
			}

        }
    }

    /* Render de bottom part of the triangle (Flat-Top) */
    inv_slope_1 = 0.f;
    inv_slope_2 = 0.f;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / fabs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / fabs(y2 - y0);

	if (y2 - y1 != 0)
	{
		for (int y = y1; y <= y2; y++)
		{
			int start_x = x1 + (y - y1) * inv_slope_1;
			int end_x = x0 + (y - y0) * inv_slope_2;

			if (start_x > end_x)
			{
				int_swap(&start_x, &end_x);     // swap if start_x is to the right of end_x
			}

			for (int x = start_x; x < end_x; x++)
			{
				draw_texel(x, y, texture, point_a, point_b, point_c, (tex2_t) { u0, v0 }, (tex2_t) { u1, v1 }, (tex2_t) { u2, v2 });
			}

		}
	}
}

/* Function to draw a rectangle */
void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

/* Function to render the color buffer */
void render_color_buffer(void)
{
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

/* Function to clear the color buffer */
void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

/* Function to clear the depth buffer */
void clear_depth_buffer(void)
{
	for (int i = 0; i < window_width * window_height; i++)
	{
		depth_buffer[i] = 1.0f;
	}
}

/* Function to destroy the window */
void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}